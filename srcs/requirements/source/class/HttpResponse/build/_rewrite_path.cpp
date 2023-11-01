/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _rewrite_path.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfoucade <yfoucade@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 10:52:02 by tda-silv          #+#    #+#             */
/*   Updated: 2023/11/01 13:48:07 by yfoucade         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

void	HttpResponse::_rewrite_path( std::string &new_path, Location *location, std::string path )
{
	if ( !location )
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
				std::cout << "_rewrite_path: str = " << str << std::endl;
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
	new_path = remove_double_slash( new_path );
}

std::string	HttpResponse::remove_double_slash( std::string &new_path )
{
	std::string ret;

	for ( size_t i = 0; i < new_path.size(); ++i )
	{
		if ( new_path[i] == '/' && new_path[i + 1] == '/')
			continue;
		ret.push_back(new_path[i]);
	}
	return ret;
}