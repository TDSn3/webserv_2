/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 16:55:13 by yfoucade          #+#    #+#             */
/*   Updated: 2023/09/12 11:22:49 by tda-silv         ###   ########.fr       */
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
	while (++first != last)
	{
		tokens = tokenize_nows(*first);
		parse_location_line(*first, tokens);
	}
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

void Location::print_location( void )
{
	std::cout << "Location: " << _uri << std::endl;
	for ( parameters_map::iterator it = _parameters.begin(); it != _parameters.end(); ++it )
	{
		std::cout << it->first << ": ";
		print_string_vector(it->second);
	}
}