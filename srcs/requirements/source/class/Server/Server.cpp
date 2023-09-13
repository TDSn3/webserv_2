/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/06 19:41:02 by yfoucade          #+#    #+#             */
/*   Updated: 2023/09/13 12:10:36 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

int Server::_n_servers = 0;

Server::Server( std::vector< std::string >::iterator first,
				std::vector< std::string >::iterator last ):
_id(_n_servers++),
_parsing_error(false)
{
	std::cout << COLOR_BLUE << "Server #" << _id << " constructor :\n" << COLOR_RESET;
	while (++first != last)
	{
		// std::cout << "processing directive: " << *first << std::endl;
		std::vector< std::string > tokens = tokenize_nows( *first );
		if (is_simple_directive(tokens))
			process_simple_directive(*first, tokens);
		else if (is_location_directive(*first, tokens))
		{
			// std::cout << "location directive\n";
			std::vector< std::string >::iterator end_of_location_block;
			end_of_location_block = find_end_of_location_block(first, last);
			// std::cout << "location block found, from:\n";
			// std::cout << *first << std::endl;
			// std::cout << "to\n";
			// std::cout << *end_of_location_block << std::endl;
			process_location_directive(first, end_of_location_block);
			first = end_of_location_block;
		}
		else if (!tokens.size())
			continue;
		else
			throw ParsingError(*first, "Invalid directive syntax");
	}
	if ( !_origins.size() )
		_origins.insert(Origin(DEFAULT_HOST, DEFAULT_PORT));
	print_config();
	std::cout << std::endl;
}

Server::Server( const Server& other ):
_id(other._id),
_parsing_error(other._parsing_error),
_origins(other._origins),
_names(other._names){}

Server::~Server(){}

void Server::print_config( void )
{
	std::cout << COLOR_DIM_BLUE << "Config of Server #" << _id << COLOR_RESET << std::endl;
	std::cout << "Origins:\n";
	for (std::set< Origin >::iterator it = _origins.begin(); it != _origins.end(); ++it)
	{
		std::cout << "\t" << it->get_host() << ":" << COLOR_BOLD_BLUE << it->get_port() << COLOR_RESET << std::endl;
	}
	std::cout << "Names:";
	for ( unsigned long i=0; i < _names.size(); ++i )
		std::cout << " " << _names[i];
	std::cout << std::endl;
	std::cout << "Locations:" << std::endl;
	for ( location_map::iterator it = _locations.begin(); it != _locations.end(); ++it)
	{
		std::cout << "\t";
		it->second.print_location();
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void	Server::parse_listen( std::string& line, std::vector<std::string> tokens )
{
	if (tokens.size() > 3)
		throw ParsingError(line, "Too many arguments for listen directive");
	std::string arg = tokens[1];
	std::ptrdiff_t n_colons = std::count(arg.begin(), arg.end(), ':');
	std::ptrdiff_t n_dots = std::count(arg.begin(), arg.end(), '.');
	std::string::size_type last_colon = arg.rfind(":");
	std::string host = DEFAULT_HOST;
	std::string port = DEFAULT_PORT;

	if ( n_colons == 1 || n_colons == 8 )
	{
		host = arg.substr(0, last_colon);
		port = arg.substr(last_colon + 1, static_cast<size_t>(-1) );
	}
	else if ( !( n_colons || n_dots ) )
		port = arg;
	else
		host = arg;
	_origins.insert(Origin(host, port));
}

void	Server::set_names( std::vector< std::string > directive_and_names )
{
	std::vector< std::string >::iterator it = directive_and_names.begin();
	std::vector< std::string >::iterator last = directive_and_names.end();

	while (++it != last)
		_names.push_back(*it);
}

void	Server::parse_server_name( std::vector<std::string> tokens)
{
	std::vector< std::string >::iterator it = tokens.begin();
	std::vector< std::string >::iterator last = tokens.end();

	while ((++it + 1) != last)
		_names.push_back(*it);
}

const std::set< Origin >& Server::get_origins( void ) const
{
	return _origins;
}

int	Server::get_id( void ) const
{
	return _id;
}

bool	Server::is_simple_directive( std::vector< std::string > tokens ) const
{
	std::vector< std::string >::iterator first = tokens.begin();
	std::vector< std::string >::iterator last = tokens.end();
	int n_words = 0;

	while ( (first != last) && ((*first)[0] != ';') )
	{
		if (is_in_charlist((*first)[0], "{}"))
			return false;
		else
		{
			++first;
			++n_words;
		}
	}
	if ( (n_words >= 2) && ((*first)[0] == ';'))
		return true;
	return false;
}


bool	Server::is_location_directive( std::string& line, std::vector< std::string > tokens ) const
{
	std::vector< std::string >::iterator first = tokens.begin();
	std::vector< std::string >::iterator last = tokens.end();

	if ( (first == last) || (first++->compare("location")) )
		return false;
	if ( (first == last) || !(is_uri(*first++)) )
		throw ParsingError(line, "Expected uri, found: " + *first);
	if ( (first == last) || (first++->compare("{")) )
		throw ParsingError(line, "Expected '{', found: " + *first);
	if ( first != last )
		throw ParsingError(line, "Expected end of line, found: " + *first);
	return true;
}

void	Server::process_simple_directive(
	std::string line,
	std::vector< std::string > tokens )
{
	if (!tokens[0].compare("listen"))
		parse_listen(line, tokens);
	else if (!tokens[0].compare("server_name"))
		parse_server_name(tokens);
	else
		throw ParsingError(line, "Unknown directive name.");
}

std::vector< std::string >::iterator Server::find_end_of_location_block(
	std::vector< std::string >::iterator first,
	std::vector< std::string >::iterator last )
{
	while ( (first != last) && (*first).compare("\t}") )
		++first;
	if ( first == last )
		throw ParsingError(*last, "Found end of server block before end of location block.");
	return first;
}

void	Server::process_location_directive(
	std::vector< std::string >::iterator curr,
	std::vector< std::string >::iterator last
)
{
	std::vector< std::string > tokens = tokenize_nows(*curr);
	if ( _locations.find(tokens[1]) != _locations.end() )
		throw ParsingError(*curr, "Location defined twice");
	Location new_location( curr, last );
	_locations[tokens[1]] = new_location;
}

bool	Server::listens_to_origin( const Origin& origin )
{
	std::set< Origin >::iterator it = _origins.begin();
	std::set< Origin >::iterator end = _origins.end();

	for ( ; it != end; ++it )
	{
		if ( it->get_port().compare(origin.get_port()) )
			continue;
		if ( (!it->get_host().compare("0.0.0.0"))
			 || (!it->get_host().compare(origin.get_host())) )
			return true;
	}
	return false;
}

bool Server::has_server_name( const std::string& name )
{
	std::vector< std::string >::iterator it = _names.begin();
	std::vector< std::string >::iterator end = _names.end();

	for ( ; it != end; ++it )
	{
		if ( !it->compare(name) )
			return true;
	}
	return false;
}

void	Server::reply( Connection& connection )
{
	connection.response.build(connection.get_request(), NULL);	// TODO: ajouter l'env;
	
	size_t	to_send =  connection.response.str_response.size();
	ssize_t	sended;

	std::cout << "Server #" << _id << " responds to connection.\n";			
	std::cout << "[" << COLOR_BOLD_GREEN << "RESPONSE" << COLOR_RESET << "]\n" << connection.response.str_response << std::endl;
	std::cout << "[" << COLOR_BOLD_RED << "END OF RESPONSE" << COLOR_RESET << "]\n" << std::endl;

	std::cout << COLOR_MAGENTA << connection.get_socket() << COLOR_RESET << "\n";
	std::cout << COLOR_MAGENTA << to_send << COLOR_RESET << "\n";
	sended = send(connection.get_socket() , connection.response.str_response.c_str() , to_send, 0);
	std::cout << COLOR_MAGENTA << "3: " << sended << COLOR_RESET << "\n";
	if (sended < static_cast<ssize_t>(to_send) )
	{
		std::cout << COLOR_BOLD_RED << "RESPONSE WAS NOT COMPLETELY SEND !" << std::endl;	// TODO: gerer cela
	}
	connection.response.clear();	
	connection.flush_request();
}
