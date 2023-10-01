/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 11:00:08 by tda-silv          #+#    #+#             */
/*   Updated: 2023/10/01 15:15:27 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

static bool	is_allowed_methods( Location *location, std::string method );

void	HttpResponse::build( Request &request, char **env, Server& server )	// ! throw possible
{
	Location					*location;
	std::vector< std::string >	accepted_methods;

	if ( request.get_final_status() == bad_request )
		my_perror_and_throw( "bad request", StatusCode( 400 ) );

	location = server.select_location( request.request_line.parsed_url.path );
	if ( location )
		location->print_location();
	else
		std::cout << COLOR_BOLD_RED << "No matching location\n" << COLOR_RESET;




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
