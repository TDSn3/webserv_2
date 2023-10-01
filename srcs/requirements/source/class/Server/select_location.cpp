/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   select_location.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfoucade <yfoucade@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/01 12:08:32 by yfoucade          #+#    #+#             */
/*   Updated: 2023/10/01 13:21:20 by yfoucade         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

Location*	Server::select_location( std::string path )
{
	location_map::iterator	location_it = _locations.begin();
	Location				*res = NULL;
	size_t					longest_match = 0;
	size_t					dot_pos;
	std::string				extension_name;
	
	std::cout << "Searching location for path: " << path << std::endl;
	if ( path.empty() )
		return NULL;
	for ( ; location_it != _locations.end(); location_it++ )
	{
		std::cout << "Comparing with: " << location_it->first << std::endl;
		dot_pos = path.rfind('.');
		if (dot_pos != std::string::npos)
		{
			extension_name = path.substr(dot_pos + 1);
			if ( extension_name == location_it->first )
				return &location_it->second;
		}
		if ( !path.compare(0, location_it->first.size(), location_it->first) )
		{
			if ( path.size() == location_it->first.size() || path[location_it->first.size()] == '/' )
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