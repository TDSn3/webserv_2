/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/06 19:41:02 by yfoucade          #+#    #+#             */
/*   Updated: 2023/11/01 10:22:00 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

int Server::_n_servers = 0;

Server::Server( std::vector< std::string >::iterator first,
				std::vector< std::string >::iterator last ):
_id(_n_servers++),
_parsing_error(false),
_max_client_body_size( DEFAULT_MAX_BODY ),
root("")
{
	std::cout << COLOR_BLUE << "Server #" << _id << " constructor :\n" << COLOR_RESET;
	while (++first != last)
	{
		// std::cout << "processing directive: " << *first << std::endl;
		std::vector< std::string > tokens = tokenize_nows( *first );
		if (is_simple_directive(tokens))
		{
			try {
				process_simple_directive(*first, tokens);
			}
			catch ( const std::exception& e ) {
				throw;
			}
		}
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
_names(other._names),
_max_client_body_size( other._max_client_body_size ),
_default_error_pages(other._default_error_pages),
_locations(other._locations),
root(other.root){}

Server::~Server(){}

void Server::print_config( void )
{
	std::cout << COLOR_DIM_BLUE << "Origins:\n" << COLOR_RESET;
	for (std::set< Origin >::iterator it = _origins.begin(); it != _origins.end(); ++it)
	{
		std::cout << "\t" << it->get_host() << ":" << COLOR_BOLD_BLUE << it->get_port() << COLOR_RESET << std::endl;
	}
	std::cout << COLOR_DIM_BLUE << "Root: " << root << COLOR_RESET;
	std::cout << COLOR_DIM_BLUE << "Names:" << COLOR_RESET;
	for ( unsigned long i=0; i < _names.size(); ++i )
		std::cout << " " << _names[i];
	std::cout << std::endl;
	std::cout << "Default Error Pages\n";
	print_map(_default_error_pages);
	// std::cout << "Max client body size: " << _max_client_body_size << std::endl;	// TODO: error valgrind
	std::cout << COLOR_DIM_BLUE<< "Locations:" << COLOR_RESET << std::endl;
	for ( location_map::iterator it = _locations.begin(); it != _locations.end(); ++it)
	{
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

void	Server::parse_server_name( std::vector<std::string> tokens )
{
	std::vector< std::string >::iterator it = tokens.begin();
	std::vector< std::string >::iterator last = tokens.end();

	while ((++it + 1) != last)
		_names.push_back(*it);
}

void	Server::parse_client_max_body_size( std::string line, std::vector<std::string> tokens )
{
	if ( !is_digit(tokens[1].c_str()) )
		throw ParsingError(line, "Expected a number.");
	_max_client_body_size = strtol(tokens[1].c_str(), NULL, 10);
	if ( errno )
		throw ParsingError(line, strerror(errno));
}

void	Server::parse_error_page( std::string line, std::vector< std::string > tokens )
{
	if ( tokens.size() < 4 )
		throw ParsingError(line, "Not enough arguments for directive.");
	if ( !is_digit(tokens[1].c_str()) )
		throw ParsingError(line, "Expected status code, got: " + tokens[1]);
	long status_code = strtol(tokens[1].c_str(), NULL, 10);
	if ( errno )
		throw ParsingError(line, strerror(errno));
	_default_error_pages[status_code] = tokens[2];
}

void	Server::parse_root( std::string line, std::vector<std::string> tokens )
{
	if ( tokens.size() < 3 )
		throw ParsingError(line, "Not enough arguments for directive.");
	root = tokens[1];
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
	else if ( tokens[0] == "client_max_body_size" )
		parse_client_max_body_size(line, tokens);
	else if ( tokens[0] == "error_page" )
		parse_error_page(line, tokens);
	else if ( tokens[0] == "root" )
		parse_root(line, tokens);
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
