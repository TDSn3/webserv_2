/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfoucade <yfoucade@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 16:55:13 by yfoucade          #+#    #+#             */
/*   Updated: 2023/09/15 23:30:53 by yfoucade         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

Location::Location(){}

/*
	Args:
		first: iterator to the header of the location block.
		last: iterator to the last line of the location block.
*/
Location::Location(
	std::vector< std::string >::iterator first,
	std::vector< std::string >::iterator last )
{
	std::string header = *first;
	std::vector< std::string > tokens = tokenize_nows(header);
	
	if (!is_uri(tokens[1]))
		throw ParsingError(header, tokens[1] + " is not a uri.");
	_uri = tokens[1];
	init_allowed_methods();
	while (++first != last)
	{
		tokens = tokenize_nows(*first);
		parse_location_line(*first, tokens);
	}
}

void	Location::init_allowed_methods( void )
{
	std::vector< std::string > allowed_methods;
	
	allowed_methods.push_back("GET");
	allowed_methods.push_back("POST");
	allowed_methods.push_back("DELETE");
	
	_parameters["allowed_methods"] = allowed_methods;
}

Location::Location( const Location& other ):
_uri(other._uri), _parameters(other._parameters)
{}


Location& Location::operator=( const Location& other )
{
	if ( this != &other )
	{
		_uri = other._uri;
		_parameters = other._parameters;
	}
	return *this;
}

Location::~Location( void ){}

void	Location::parse_location_line(
	 std::string line,
	 std::vector< std::string > tokens )
{
	if ( tokens.size() == 0 )
		return;
	else if ( !is_simple_directive(tokens) )
		throw ParsingError(line, "Invalid directive format.");
	if ( !tokens[0].compare("root"))
		set_root(line, tokens);
	if ( tokens[0] == "allowed_methods" )
		parse_allowed_methods(line, tokens);
	if ( tokens[0] == "return" )
		parse_return(line, tokens);
	if ( tokens[0] == "autoindex" )
		parse_autoindex(line, tokens);
	if ( tokens[0] == "index" )
		parse_index(line, tokens);
	if ( tokens[0] == "cgi" )
		parse_cgi(line, tokens);
	if ( tokens[0] == "upload_store" )
		parse_upload_store(line, tokens);
}

void	Location::set_root( std::string line, std::vector< std::string > tokens )
{
	std::cout << "set_root: got: " << line << std::endl;
	if ( tokens.size() > 3)
		throw ParsingError(line, "Too many arguments for root directive.");
	if ( _parameters.find("root") != _parameters.end() )
		throw ParsingError(line, "Parameter 'root' defined twice.");
	_parameters["root"] = std::vector< std::string >(tokens.begin() + 1, tokens.end() - 1);
}

void	Location::parse_allowed_methods( std::string line, std::vector< std::string > tokens)
{
	(void)line;
	_parameters["allowed_methods"] = std::vector< std::string >( tokens.begin() + 1, tokens.end() - 1 );
}

void	Location::parse_return( std::string line, std::vector< std::string > tokens )
{
	if ( tokens.size() < 3 )
		throw ParsingError(line, "Missing arguments for 'return' directive.");
	_parameters["return"] = std::vector< std::string >(tokens.begin() + 1, tokens.begin() + 2);
}

void	Location::parse_autoindex( std::string line, std::vector< std::string > tokens )
{
	if ( tokens.size() < 3 )
		throw ParsingError(line, "Missing arguments for 'autoindex' directive.");
	_parameters["autoindex"] = std::vector< std::string >(tokens.begin() + 1, tokens.begin() + 2);
}

void	Location::parse_index( std::string line, std::vector< std::string > tokens )
{
	if (tokens.size() < 3 )
		throw ParsingError(line, "Missing arguments for 'index' directive.");
	_parameters["index"] = std::vector< std::string >(tokens.begin() + 1, tokens.end() - 1);
}

void	Location::parse_cgi( std::string line, std::vector< std::string > tokens )
{
	(void)line;
	_parameters["cgi"] = std::vector< std::string >(tokens.begin() + 1, tokens.end() - 1);
}

void	Location::parse_upload_store( std::string line, std::vector< std::string > tokens )
{
	if ( tokens.size() < 3 )
		throw ParsingError(line, "Missing arguments for 'upload_store' directive.");
	_parameters["upload_store"] = std::vector< std::string >(tokens.begin() + 1, tokens.end() - 1);
}

void Location::print_location( void )
{
	std::cout << "Location: " << _uri << std::endl;
	for ( parameters_map::iterator it = _parameters.begin(); it != _parameters.end(); ++it )
	{
		std::cout << it->first << ": ";
		print_string_vector(it->second);
	}
}