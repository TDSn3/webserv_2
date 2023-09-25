/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfoucade <yfoucade@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 14:26:42 by yfoucade          #+#    #+#             */
/*   Updated: 2023/09/14 23:50:26 by yfoucade         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <vector>
#include <string>

void	print_string_vector( std::vector< std::string > vec, std::string sep = " " );
template< class Key, class T >
void	print_map( std::map< Key, T > map );

template< typename Key, typename T >
void	print_map( std::map< Key, T > map )
{
	typename std::map< Key, T >::iterator it = map.begin();
	typename std::map< Key, T >::iterator end = map.end();
	
	for ( ; it != end; ++it )
		std::cout << it->first << ": " << it->second << std::endl;
}
