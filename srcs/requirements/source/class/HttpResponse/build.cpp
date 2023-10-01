/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 11:00:08 by tda-silv          #+#    #+#             */
/*   Updated: 2023/10/01 15:57:58 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

static bool	is_allowed_methods( Location *location, std::string method );
static void	rewrite_path( std::string &new_path, Location *location, std::string path );

void	HttpResponse::build( Request &request, char **env, Server& server )	// ! throw possible
{
	Location					*location;
	std::vector< std::string >	accepted_methods;
	std::string					new_path;

	if ( request.get_final_status() == bad_request )
		my_perror_and_throw( "bad request", StatusCode( 400 ) );

	location = server.select_location( request.request_line.parsed_url.path );
	if ( location )
		location->print_location();
	else
		std::cout << COLOR_BOLD_RED << "No matching location\n" << COLOR_RESET;
	
	
	
	rewrite_path( new_path, location, request.request_line.parsed_url.path );
	//std::cout << COLOR_BOLD_GREEN << "============> " << new_path << COLOR_RESET << "\n";

	if ( is_allowed_methods( location, request.request_line.method ) == true )
	{
		if ( request.request_line.method == "GET" )
		{
			if ( _get_method( request, env ) == true )			// ! throw possible
				return ;										// Le CGI a rempli la réponse
		}
		else if ( request.request_line.method == "POST" )
		{
			if ( _post_method( request, env ) == true )			// ! throw possible
				return ;										// TODO: add parsing on body
		}
		else if ( request.request_line.method == "DELETE" )
			_delete_method( request );
		else
			my_perror_and_throw( "method not allowed", StatusCode( 405 ) );
	}
	else
		my_perror_and_throw( "method not allowed", StatusCode( 405 ) );

	_make_response( request );								// ! throw possible
};

static bool	is_allowed_methods( Location *location, std::string method )
{	
	if ( !location)
		return ( method == "GET" || method == "POST" || method == "DELETE" );
	else
	{	
		if ( location->_parameters.find( "allowed_methods" ) != location->_parameters.end() )
		{
			std::vector< std::string > :: iterator it = location->_parameters[ "allowed_methods" ].begin();

			while ( it != location->_parameters[ "allowed_methods" ].end() )
			{
				if ( *it == method )
					return ( true );
				it++;
			}
			
		}
	}

	return ( false );
}

static void	rewrite_path( std::string &new_path, Location *location, std::string path )
{
	if ( !location)
		new_path = path;
	else
	{
		if ( location->_parameters.find( "root" ) != location->_parameters.end() )
		{
			if ( location->_parameters[ "root" ].empty() == false )
			{
				std::string	str;
				
				new_path = std::string( ROOT ) + "/" + location->_parameters[ "root" ][0];
				str = path.substr( location->_uri.size() );
				if (str.size() > 0 )
					new_path += "/" + str;
			}
		}
		else
			new_path = path;
	}
}
