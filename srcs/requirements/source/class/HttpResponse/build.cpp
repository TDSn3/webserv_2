/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfoucade <yfoucade@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 11:00:08 by tda-silv          #+#    #+#             */
/*   Updated: 2023/10/02 05:10:56 by yfoucade         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

static bool	is_allowed_methods( Location *location, std::string method );
static void	rewrite_path( std::string &new_path, Location *location, std::string path );

bool	is_directory( std::string name )
{
	DIR	*directory = opendir(name.c_str());
	if ( !directory )
		return false;
	closedir(directory);
	return true;
}

bool	is_in_directory( std::string dir, std::string file )
{
	DIR				*directory = opendir(dir.c_str());
	struct dirent	*dir_entry;
	bool			ret = false;

	if ( !directory )
		return false;
	while ( (dir_entry = readdir(directory)) )
	{
		if ( file == dir_entry->d_name )
		{
			ret = true;
			break;
		}
	}
	closedir(directory);
	return ret;
}

bool	autoindex_is_on( Location *location )
{
	if ( !location
	|| location->_parameters.find("autoindex") == location->_parameters.end()
	|| location->_parameters["autoindex"][0] != "on" )
		return false;
	return true;
}

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
	
	
	new_path = server.root;
	rewrite_path( new_path, location, request.request_line.parsed_url.path );
	std::cout << COLOR_BOLD_GREEN << "path : " << new_path << COLOR_RESET << "\n";

	if ( is_directory(new_path) )
	{
		if ( !autoindex_is_on(location) )
			my_perror_and_throw( "Page not found", StatusCode( 404 ) );
		else
		{
			
			// TODO: directory listing
			;
		}
	}

	if ( is_allowed_methods( location, request.request_line.method ) == true )
	{
		if ( request.request_line.method == "GET" )
		{
			if ( _get_method( request, env, new_path ) == true )	// ! throw possible
				return ;											// Le CGI a rempli la réponse
		}
		else if ( request.request_line.method == "POST" )
		{
			if ( _post_method( request, env, new_path ) == true )	// ! throw possible
				return ;											// TODO: add parsing on body
		}
		else if ( request.request_line.method == "DELETE" )
			_delete_method( new_path );
		else
			my_perror_and_throw( "method not allowed", StatusCode( 405 ) );
	}
	else
		my_perror_and_throw( "method not allowed", StatusCode( 405 ) );

	_make_response( new_path );							// ! throw possible
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
		new_path += path;
	else
	{
		if ( location->_parameters.find( "root" ) != location->_parameters.end() )
		{
			if ( location->_parameters[ "root" ].empty() == false )
			{
				std::string	str;
				
				new_path += location->_parameters[ "root" ][0];
				str = path.substr( location->_uri.size() );
				if (str.size() > 0 )
					new_path += "/" + str;
			}
		}
		else
			new_path += path;
	}
	
	std::cout << "new_path: " << new_path << std::endl;
	if ( is_directory(new_path) && ( location->_parameters.find( "index" ) != location->_parameters.end() ) )
	{
		std::vector< std::string >::iterator files_it = location->_parameters["index"].begin();
		for ( ; files_it != location->_parameters["index"].end(); ++files_it )
		{
			if ( is_in_directory(new_path, *files_it) )
			{
				new_path += "/" + *files_it;
				break;
			}
		}
	}
}
