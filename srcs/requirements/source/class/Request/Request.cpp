/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfoucade <yfoucade@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 11:59:07 by yfoucade          #+#    #+#             */
/*   Updated: 2023/10/02 12:24:41 by yfoucade         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

Request::Request():
_parsing_status(e_request_line),
_final_status(parsing),
_has_host(false),
_has_content_length(false),
_has_transfer_encoding(false),
_has_expect(false),
_has_connection(false)
{}

Request::Request( const Request& other )
{
	*this = other;
}

Request& Request::operator=( const Request& other )
{
	if ( this != &other )
	{
		_parsing_status = other._parsing_status;
		_final_status = other._final_status;
		_content = other._content;
		_nchars_remaining = other._nchars_remaining;
		_chunk_size_cumsum = other._chunk_size_cumsum;
		request_line = other.request_line;
		_raw_header_section = other._raw_header_section;
		_raw_trailer_section = other._raw_trailer_section;
		_header_section = other._header_section;
		_trailer_section = other._trailer_section;
		_has_host = other._has_host;
		_host_value = other._host_value;
		_has_content_length = other._has_content_length;
		_content_length_value = other._content_length_value;
		_has_transfer_encoding = other._has_transfer_encoding;
		_transfer_encoding_value = other._transfer_encoding_value;
		_has_expect = other._has_expect;
		_expect_value = other._expect_value;
		_has_connection = other._has_connection;
		_connection_value = other._connection_value;
		_body = other._body;
		_last_reception = other._last_reception;
	}
	return *this;
}

Request::~Request( void ){}

bool Request::is_complete( void )
{
	return _parsing_status == done;
}

std::string::size_type Request::get_required_size( void )
{
	if ( _parsing_status == body || _parsing_status == chunk_data )
		return _nchars_remaining + (_parsing_status == chunk_data ? 2 : 0);
	return LINE;
}

void	Request::add_data( const std::string & line )
{
	// TODO: update _last_reception

	switch (_parsing_status)
	{
		case e_request_line:
			parse_request_line(line);
			break;
		case header_section:
			add_header_line(line);
			break;
		case body:
			parse_body(line);
			break;
		case chunk_size:
			parse_chunk_size(line);
			break;
		case chunk_data:
			parse_chunk_content(line);
			break;
		case trailer_section:
			add_trailer_line(line);
			break;
		default :
			break;
	}
}

void	Request::parse_request_line( const std::string & line)
{
	if ( line.size() == 0 )
		return;
	request_line = RequestLine(line);
	// request_line.print_request_line();
	_parsing_status = header_section;
}

void	Request::add_header_line( const std::string & line)
{
	// TODO: if empty line, decide whether status is:
	// - done
	// - expect_100_continue
	// - body
	// - chunk_size

	if ( line.size() == 0 )
	{
		if ( request_line.has_syntax_error() )
		{
			_parsing_status = done;
			_final_status = bad_request;
			return;
		}
		parse_field_section(_raw_header_section, _header_section);
		if ( set_field_values( _header_section ) )
			return;
		// print_field_section(_header_section);
		// todo: Check that each field-value is correct, depending on the field's requirements
		_parsing_status = get_status_after_header();
		return;
	}
	// add_field_to_section(line, _header_section);
	_raw_header_section.push_back(line);
}

void
Request::parse_field_section( std::vector< std::string >& raw_field_section,
							  field_section_type& field_section )
{
	std::vector< std::string >::iterator it = raw_field_section.begin();
	std::vector< std::string >::iterator end = raw_field_section.end();

	for ( ; it != end; ++it )
		add_field_to_section( *it, field_section );
}

bool	Request::set_field_values( field_section_type& field_section )
{
	if ( set_host_field( field_section ) )
		return FAILURE;
	if ( set_content_length_field( field_section ) )
		return FAILURE;
	if ( set_transfer_encoding_field( field_section ) )
		return FAILURE;
	if ( set_expect_field( field_section ) )
		return FAILURE;
	if ( set_connection_field( field_section ) )
		return FAILURE;
	return SUCCESS;
}


bool	Request::set_host_field( field_section_type& field_section )
{
	field_section_type::iterator host_field = field_section.find("host");

	// missing host field
	if ( host_field == field_section.end() && !_has_host )
	{
		_parsing_status = done;
		_final_status = bad_request;
		return FAILURE;
	}
	// multiple host fields (header + trailer)
	else if ( host_field != field_section.end() && _has_host )
	{
		_parsing_status = done;
		_final_status = bad_request;
		return FAILURE;
	}
	// multiple host values
	else if ( (host_field != field_section.end()) && (host_field->second.size() > 1) )
	{
		_parsing_status = done;
		_final_status = bad_request;
		return FAILURE;
	}
	else
	{
		_has_host = true;
		_host_value = host_field->second[0];
		return SUCCESS;
	}
}

bool	Request::set_content_length_field( field_section_type& field_section )
{
	field_section_type::iterator content_length_field = field_section.find("content-length");
	long content_length_value;

	if ( content_length_field == field_section.end() )
		return SUCCESS;
	// multiple content-length
	if ( _has_content_length
		|| _has_transfer_encoding
		|| content_length_field->second.size() > 1 )
	{
		_parsing_status = done;
		_final_status = bad_request;
		return FAILURE;
	}
	if ( !is_digit( content_length_field->second[0].c_str() ))
	{
		_parsing_status = done;
		_final_status = bad_request;
		return FAILURE;
	}
	content_length_value = std::strtol(
		content_length_field->second[0].c_str(),
		NULL,
		10 );
	if ( content_length_value == LONG_MAX )
	{
		_parsing_status = done;
		_final_status = bad_request;
		return FAILURE;
	}
	_has_content_length = true;
	_content_length_value = content_length_value;
	return SUCCESS;
}

bool	Request::set_transfer_encoding_field( field_section_type& field_section )
{
	field_section_type::iterator transfer_encoding_field = field_section.find("transfer-encoding");
	
	if ( transfer_encoding_field == field_section.end() )
		return SUCCESS;
	if ( _has_transfer_encoding
		 || _has_content_length
		 || transfer_encoding_field->second.size() > 1
		 || transfer_encoding_field->second[0].compare("chunked") )
	{
		_parsing_status = done;
		_final_status = bad_request;
		return FAILURE;
	}
	_has_transfer_encoding = true;
	_transfer_encoding_value = "chunked";
	return SUCCESS;
}

bool	Request::set_expect_field( field_section_type& field_section )
{
	field_section_type::iterator expect_field = field_section.find("expect");

	if ( expect_field == field_section.end() )
		return SUCCESS;
	if ( _has_expect
		 || expect_field->second.size() > 1
		 || expect_field->second[0].compare("100-continue") )
	{
		_parsing_status = done;
		_final_status = bad_request;
		return FAILURE;
	}
	_has_expect = true;
	_expect_value = "100-continue";
	return SUCCESS;
}

bool	Request::set_connection_field( field_section_type& field_section )
{
	field_section_type::iterator connection_field = field_section.find("connection");
	

	if ( connection_field == field_section.end() )
		return SUCCESS;
	if ( _has_connection
		 || connection_field->second.size() > 1 )
	{
		_parsing_status = done;
		_final_status = bad_request;
		return FAILURE;
	}
	std::string connection_value = connection_field->second[0];
	ft_tolower(connection_value);
	_has_connection = true;
	_connection_value = connection_value;
	return SUCCESS;
}


void	Request::add_field_to_section( const std::string line, field_section_type& section)
{
	check_field_syntax(line);
	if ( _parsing_status == done )
		return ;
	std::string field_name = line.substr(0, line.find(':'));
	ft_tolower(field_name);
	// Check that field name is recognized, otherwise ignore
	std::string field_value = line.substr(line.find(':') + 1, line.size() - line.find(':') - 1);
	trim(field_value);
	std::vector< std::string >& field_values = section[field_name];
	field_values.push_back(field_value);
}

void	Request::check_field_syntax( const std::string& line )
{
	std::string::const_iterator it = line.begin();
	std::string::const_iterator last = line.end();

	while ( (it != last) && (*it != ':') )
	{
		if ( !is_tchar(*it++) )
		{
			_parsing_status = done;
			_final_status = bad_request;
			return;
		}
	}
	if ( (it == line.begin()) || (it++ == last) )
	{
		_parsing_status = done;
		_final_status = bad_request;
		return;
	}
	while ( (it != last) && (is_whitespace(*it) || is_field_vchar(*it)) )
		++it;
	if ( it != last )
	{
		_parsing_status = done;
		_final_status = bad_request;
		return;
	}
}

void	Request::parse_body( const std::string & line)
{
	std::cout << "parse_body: got: " << line << std::endl;
	if ( line.size() != _nchars_remaining )
	{
		_parsing_status = done;
		_final_status = bad_request;
		return;
	}
	_body = line;
	_parsing_status = done;
	_final_status = success;
}

void	Request::parse_chunk_size( const std::string & line)
{
	
	// TODO: check that whole line's syntax is correct
	std::string::const_iterator size_end = line.begin();
	
	while ( (size_end != line.end()) && is_hexdigit(*size_end) )
		++size_end;
	std::string size_str = std::string(line.begin(), size_end);
	if ( size_str.size() == 0 )
	{
		_parsing_status = done;
		_final_status = bad_request;
		return;
	}
	long size;
	std::stringstream ss;
	ss << std::hex << size_str; // TODO: handle error
	ss >> size;
	// long size = std::strtol(size_str.c_str(), NULL, 10);
	// if ( size == LONG_MAX )
	// {
	// 	_parsing_status = done;
	// 	_final_status = bad_request;
	// 	return;
	// }
	_nchars_remaining = static_cast< std::string::size_type >(size);
	if ( _nchars_remaining == 0 )
	{
		_parsing_status = trailer_section;
		_has_content_length = true;
		_content_length_value = _body.size();
	}
	_parsing_status = _nchars_remaining == 0 ? trailer_section : chunk_data;
}

void	Request::parse_chunk_content( const std::string & line)
{
	
	if ( line.size() != _nchars_remaining + 2 ) // at least 1 char + CRLF
	{
		_parsing_status = done;
		_final_status = bad_request;
		return ;
	}
	std::string::const_iterator it = line.end();
	
	if ( (*--it != '\n') || (*--it != '\r') )
	{
		_parsing_status = done;
		_final_status = bad_request;
		return ;
	}
	
	_body += std::string(line.begin(), it);
	_chunk_size_cumsum += _nchars_remaining;
	_parsing_status = chunk_size;
}

void	Request::add_trailer_line( const std::string & line)
{
	// TODO: parse trailer section
	std::cout << "parse_trailer_line: got: " << line << std::endl;
	if ( line.size() == 0 )
	{
		_parsing_status = done;
		_final_status = success;
	}
}

e_parsing_status Request::get_status_after_header( void )
{
	if ( !_has_content_length && !_has_transfer_encoding )
	{
		_final_status = success;
		return done;
	}
	if ( _has_expect && ( _has_transfer_encoding || _content_length_value > 0 ) )
		return expect_100_continue;
	if ( _has_transfer_encoding )
		return chunk_size;
	if ( _has_content_length )
	{
		_nchars_remaining = static_cast< std::string::size_type >(_content_length_value);
		return body;
	}
	_final_status = success;
	return done;
}

void	Request::set_status_after_continue( void )
{
	if ( _has_transfer_encoding )
		_parsing_status = chunk_size;
	else
	{
		_nchars_remaining = static_cast< std::string::size_type >(_content_length_value);
		_parsing_status = body;
	}
}

std::string Request::get_field_value( std::string field_name )
{
	if ( !_header_section.count(field_name) )
		return std::string();
	return _header_section.find(field_name)->second[0];
}

bool	Request::is_valid_content_length( std::string content_length )
{
	std::string::iterator it = content_length.begin();
	std::string::iterator last = content_length.end();

	if ( content_length.size() == 0 )
		return false;
	for ( ; it != last; ++it )
	{
		if ( !isdigit(*it) )
			return false;
	}
	return true;
}

bool	Request::get_parsing_error( void )
{
	// return _parsing_error;
	return ( (_final_status != parsing) && (_final_status != success) );
}

bool	Request::has_close_option( void )
{
	std::map< std::string, std::vector< std::string > >::iterator it;
	
	it = _header_section.find("Connection");
	if ( it == _header_section.end() || it->second[0].compare("close") )
		return false;
	return true;
}

e_parsing_status Request::get_parsing_status( void )
{
	return _parsing_status;
}

void	Request::print_field_section( field_section_type& fields )
{
	field_section_type::iterator it = fields.begin();
	field_section_type::iterator end = fields.end();
	
	for ( ; it != end; ++it )
	{
		std::cout << it->first << ": ";
		print_string_vector(it->second, ", ");
	}
}

std::string	Request::get_host_value( void )
{
	return _host_value;
}
