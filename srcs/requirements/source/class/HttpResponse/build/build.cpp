/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 11:00:08 by tda-silv          #+#    #+#             */
/*   Updated: 2023/10/02 11:19:23 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

void	HttpResponse::build( Request &request, char **env, Server& server )	// ! throw possible
{
	Location	*location;
	std::string	new_path;

	if ( request.get_final_status() == bad_request )
		my_perror_and_throw( "bad request", StatusCode( 400 ) );

	if ( request.get_content_length_status() && request.get_content_length_value() > server._max_client_body_size )
		my_perror_and_throw( "bad request", StatusCode( 413 ) );

	location = server.select_location( request.request_line.parsed_url.path );

	if ( location )
		location->print_location();
	else
		std::cout << COLOR_BOLD_RED << "No matching location\n" << COLOR_RESET;

	new_path = server.root;
	_rewrite_path( new_path, location, request.request_line.parsed_url.path );

	std::cout << COLOR_BOLD_GREEN << "new_path : " << new_path << COLOR_RESET << "\n";

	if ( is_directory(new_path) )
	{
		if ( !_autoindex_is_on (location ) )
			my_perror_and_throw( "Page not found", StatusCode( 404 ) );
		else
		{
			
			// TODO: directory listing
			;
		}
	}

	if ( _select_method( request, env, location, new_path ) == true )	// ! throw possible
		return ;

	_make_response( new_path );											// ! throw possible
};

