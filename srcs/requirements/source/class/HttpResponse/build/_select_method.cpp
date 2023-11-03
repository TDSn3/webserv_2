/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _select_method.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 10:58:54 by tda-silv          #+#    #+#             */
/*   Updated: 2023/11/03 11:55:24 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

bool	HttpResponse::_select_method( Gateway &gateway, Request &request, char **env, Location *location, std::string path, Server &server )	// ! throw possible
{
	if ( is_allowed_methods( location, request.request_line.method ) == true )
	{			
		if ( location && location->_parameters.find( "cgi" ) != location->_parameters.end() )
		{
			std::cout << COLOR_BOLD_CYAN << "CGI" << COLOR_RESET;
			if ( location->_parameters[ "cgi" ].empty() == false )
			{
				std::cout << COLOR_BOLD_CYAN << " : " << location->_parameters[ "cgi" ][0] << COLOR_RESET << std::endl;
				str_response = _exec_cgi( gateway, location->_parameters[ "cgi" ][0], path, request, env, server );			// ! throw possible
				return ( true );
			}
			else
				std::cout << std::endl;
		}
		else if ( location && location->_uri[0] == '.' )
		{
			std::cout << COLOR_BOLD_CYAN << "CGI" << COLOR_RESET;
			{
				std::cout << COLOR_BOLD_CYAN << " : " << path << COLOR_RESET << std::endl;
				str_response = _exec_cgi( gateway, path, path, request, env, server );			// ! throw possible
				return ( true );
			}
		}

		if ( request.request_line.method == "GET" )
		{
			if ( _get_method( request, env, path, server ) == true )	// ! throw possible
				return ( true );								// Le CGI a rempli la réponse
		}
		else if ( request.request_line.method == "POST" )
		{
			if ( _post_method( request, env, path, *location, server ) == true )	// ! throw possible
				return ( true );
		}
		else if ( request.request_line.method == "DELETE" )
			_delete_method( path );
		else
			my_perror_and_throw( "method not allowed", StatusCode( 405 ) );
	}
	else
		my_perror_and_throw( "method not allowed", StatusCode( 405 ) );
	
	return ( false );
}
