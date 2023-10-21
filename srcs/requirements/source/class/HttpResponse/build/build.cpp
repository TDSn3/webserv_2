/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfoucade <yfoucade@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 11:00:08 by tda-silv          #+#    #+#             */
/*   Updated: 2023/10/21 10:33:20 by yfoucade         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

void	HttpResponse::build( Request &request, char **env, Server& server )	// ! throw possible
{
	Location	*location;
	std::string	new_path;

	// TODO: add 100 continue

	if ( request.get_final_status() == bad_request )
		my_perror_and_throw( "bad request", StatusCode( 400 ) );

	location = server.select_location( request.request_line.parsed_url.path, request.request_line.method );

	if ( location )
	{
		if ( location->_parameters.find( "client_max_body_size" ) != location->_parameters.end() )
		{
			std::istringstream	iss( location->_parameters[ "client_max_body_size" ][0] );
			size_t				client_max_body_size;
			
			iss >> client_max_body_size ;
			if ( request.get_body().size() > client_max_body_size )
				my_perror_and_throw( "Content Too Large", StatusCode( 413 ) );
		}
		location->print_location();
	}
	else // TODO: return 404 ?
		std::cout << COLOR_BOLD_RED << "No matching location\n" << COLOR_RESET; // TODO: reply with error

	new_path = server.root;
	// also appends default_file
	_rewrite_path( new_path, location, request.request_line.parsed_url.path );

	// TODO: nginx considers that the initially requested resource is a directory iff
	// it ends with the character '/'.
	// Question: are POST and DELETE requests allowed on directories ?
	if ( is_directory(new_path) ) // no default_file found
	{
		std::cout << "is_directory\n";
		if ( !_autoindex_is_on( location ) )
			my_perror_and_throw( "Page not found", StatusCode( 404 ) );
		else
		{
			str_body = DirectoryListing( new_path, request.request_line.parsed_url.path ).get_html();
			_set_status_line( 200, "OK" );
		}
	}

	else if ( _select_method( request, env, location, new_path ) == true )	// ! throw possible
	{
		to_send =  str_response.size();
		sent = 0;
		total_sent = 0;

		return ;
	}

	_make_response( new_path );											// ! throw possible

	to_send =  str_response.size();
	sent = 0;
	total_sent = 0;
};

