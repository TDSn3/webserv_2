/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestLine.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfoucade <yfoucade@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 11:25:27 by tda-silv          #+#    #+#             */
/*   Updated: 2023/11/01 21:05:59 by yfoucade         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

RequestLine::RequestLine( void ):
_syntax_is_ok(true)
{}

RequestLine::RequestLine( const std::string& line ):
_syntax_is_ok(true)
{
	std::string::const_iterator it = line.begin();
	std::string::const_iterator end = line.end();

	it = extract_method(it, end);
	it = extract_target(it, end);
	it = extract_version(it, end);
	_parse_url();
}

RequestLine::RequestLine( const RequestLine& other )
{
	*this = other;
}

RequestLine& RequestLine::operator=( const RequestLine& other )
{
	if ( this != &other )
	{
		method = other.method;
		_request_target = other._request_target;
		_http_version = other._http_version;
		_syntax_is_ok = other._syntax_is_ok;
		parsed_url = other.parsed_url;
		parsed_url.query_parameters = other.parsed_url.query_parameters;
	}
	return *this;
}

RequestLine::~RequestLine( void )
{}

std::string::const_iterator
RequestLine::extract_method( std::string::const_iterator first, std::string::const_iterator last )
{
	std::string::const_iterator it = first;

	if ( (it == last) || (!is_tchar(*it)) )
	{
		_syntax_is_ok = false;
		return last;
	}
	while ( (it != last) && (is_tchar(*it)) )
		++it;
	if ( (it == last) || (*it != ' ') )
	{
		_syntax_is_ok = false;
		return last;
	}
	method = std::string(first, it);
	return ++it;
}

std::string::const_iterator
RequestLine::extract_target( std::string::const_iterator first, std::string::const_iterator last )
{
	std::string::const_iterator it = first;
	// extract absolute path
	while ( (it != last) && (*it == '/') )
		it = go_to_segment_end(it + 1, last);
	// no initial '/'
	if ( (it == first) )
	{
		_syntax_is_ok = false;
		return last;
	}
	// extract query
	if ( (it != last) && (*it == '?') )
	{
		++it;
		while ( (it != last) && (is_pchar(it, last) || (*it == '/') || (*it == '?')) )
			++it;
	}
	if ( (it == last) || (*it != ' ') )
	{
		_syntax_is_ok = false;
		return last;
	}
	_request_target = std::string(first, it);
	return ++it;
}

std::string::const_iterator
RequestLine::go_to_segment_end( std::string::const_iterator first, std::string::const_iterator last )
{
	while ( is_pchar(first, last) )
		++first;
	return first;
}

std::string::const_iterator
RequestLine::extract_version( std::string::const_iterator first, std::string::const_iterator last )
{
	std::string::const_iterator it = first;
	std::string str(first, last);

	if ( str.find("HTTP/") != 0 )
	{
		_syntax_is_ok = false;
		return last;
	}
	it += 5;
	if ( (it == last) || (!is_digit(*it++)) )
	{
		_syntax_is_ok = false;
		return last;
	}
	_major_version = *it - '0';
	if ( (it == last) || (*it++ != '.') )
	{
		_syntax_is_ok = false;
		return last;
	}
	if ( (it == last) || (!is_digit(*it++)) )
	{
		_syntax_is_ok = false;
		return last;
	}
	_minor_version = *it - '0';
	_http_version = std::string(first, it);
	if ( it != last )
		_syntax_is_ok = false;
	return last;
}

void	RequestLine::print_request_line( std::string before, std::string after )
{
	std::cout << before << (before.size() ? "\n" : "");
	std::cout << "Method: " << method;
	std::cout << ", target: " << _request_target;
	std::cout << ", version: " << _http_version << std::endl;
	std::cout << after << (after.size() ? "\n" : "");
}

void	RequestLine::print_request_line_two( void )
{
	std::cout << method 			<< " ";
	std::cout << _request_target	<< " ";
	std::cout << _http_version		<< "\n";
}

int RequestLine::get_major_version( void )
{
	return _major_version;
}

int RequestLine::get_minor_version( void )
{
	return _minor_version;
}

std::string RequestLine::get_version( void )
{
	return _http_version;
}

bool RequestLine::has_syntax_error( void )
{
	return !_syntax_is_ok;
}