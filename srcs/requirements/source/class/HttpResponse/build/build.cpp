/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 11:00:08 by tda-silv          #+#    #+#             */
/*   Updated: 2023/11/03 11:45:51 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

void	HttpResponse::build( Gateway &gateway, Request &request, char **env, Server& server )	// ! throw possible
{
	Location	*location;
	Location	*cgi_location;
	std::string	new_path;

	if ( request.get_final_status() == bad_request )
		my_perror_and_throw( "bad request", StatusCode( 400 ) );

	location = server.select_non_cgi_location( request.request_line.parsed_url.path );
	cgi_location = server.select_cgi_location( request.request_line.parsed_url.path, request.request_line.method );

	if ( location )
	{
		if ( location->_parameters.find("rewrite") != location->_parameters.end() )
		{
			str_response = "HTTP/1.1 307 Temporary Redirect\r\n";
			_add_field_line( "Location", request.request_line.parsed_url.path.replace(0, location->_uri.size(), location->_parameters["rewrite"][0]) );
			str_response += "\r\n";
			to_send =  str_response.size();
			sent = 0;
			total_sent = 0;
			return ;
		}
		if ( location->_parameters.find("return") != location->_parameters.end() )
		{
			str_response = "HTTP/1.1 308 Permanent Redirect\r\n";
			_add_field_line( "Location", request.request_line.parsed_url.path.replace(0, location->_uri.size(), location->_parameters["return"][0]) );
			str_response += "\r\n";
			to_send =  str_response.size();
			sent = 0;
			total_sent = 0;
			return ;
		}
		if ( location->_parameters.find( "client_max_body_size" ) != location->_parameters.end() )
		{
			std::istringstream	iss( location->_parameters[ "client_max_body_size" ][0] );
			size_t				client_max_body_size;
			
			iss >> client_max_body_size ;
			if ( request.get_body().size() > client_max_body_size )
				my_perror_and_throw( "Content Too Large", StatusCode( 413 ) );
		}
		location->print_location();
		new_path = server.root;
		// also appends default_file
		_rewrite_path( new_path, location, request.request_line.parsed_url.path );
	}
	if ( cgi_location )
	{
		location = cgi_location;
	}
	if ( !location )
	{
		std::cout << COLOR_BOLD_RED << "No matching location\n" << COLOR_RESET;
		my_perror_and_throw( "Not Found", StatusCode( 404 ) );
	}

	std::cout << COLOR_BOLD_YELLOW << new_path << "\n" << COLOR_RESET;
	std::cout << COLOR_BOLD_YELLOW << request.request_line.parsed_url.path << "\n" << COLOR_RESET;

	if ( is_directory(new_path) && request.request_line.method == "GET" ) // no default_file found
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

	else if ( _select_method( gateway, request, env, location, new_path, server ) == true )	// ! throw possible
	{
		to_send =  str_response.size();
		sent = 0;
		total_sent = 0;

		return ;
	}

	_make_response( request, new_path );									// ! throw possible

	to_send =  str_response.size();
	sent = 0;
	total_sent = 0;
};

