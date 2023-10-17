/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _rewrite_path.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 10:52:02 by tda-silv          #+#    #+#             */
/*   Updated: 2023/10/02 10:52:54 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

void	HttpResponse::_rewrite_path( std::string &new_path, Location *location, std::string path )
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
