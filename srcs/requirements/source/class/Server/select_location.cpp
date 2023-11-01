/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   select_location.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfoucade <yfoucade@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/01 12:08:32 by yfoucade          #+#    #+#             */
/*   Updated: 2023/11/01 12:20:54 by yfoucade         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

Location*	Server::select_non_cgi_location( std::string path )
{
	location_map::iterator	location_it = _locations.begin();
	Location				*res = NULL;
	size_t					longest_match = 0;
	
	std::cout << "Searching location for path: " << path << std::endl;
	if ( path.empty() )
		return NULL;
	for ( ; location_it != _locations.end(); location_it++ )
	{
		std::cout << "Comparing with: " << location_it->first << std::endl;
		if ( !path.compare(0, location_it->first.size(), location_it->first) )
		{
			if ( location_it->first.size() == 1 ||  path.size() == location_it->first.size() || path[location_it->first.size()] == '/' )
			{
				if ( location_it->first.size() > longest_match )
				{
					longest_match = location_it->first.size();
					res = &location_it->second;
				}
			}
		}
	}
	return res;
}

Location*	Server::select_cgi_location( std::string path, std::string method )
{
	location_map::iterator	location_it = _locations.begin();
	Location				*res = NULL;
	size_t					dot_pos;
	std::string				extension_name;
	
	std::cout << "Searching location for path: " << path << std::endl;
	dot_pos = path.find('.') == std::string::npos;
	if ( path.empty() || dot_pos )
		return NULL;
	for ( ; location_it != _locations.end(); location_it++ )
	{
		if ( !(location_it->first[0] == '.') )
			continue;

		std::cout << "Comparing with: " << location_it->first << std::endl;
		if (	path.substr( dot_pos, location_it->first.size() ) == location_it->first 
			&&	is_allowed_methods(&location_it->second, method) )
			return &location_it->second;
	}
	return res;
}