/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_allowed_methods.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfoucade <yfoucade@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 10:49:16 by tda-silv          #+#    #+#             */
/*   Updated: 2023/10/02 12:07:48 by yfoucade         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

bool	is_allowed_methods( Location *location, std::string method )
{	
	if ( !location || location->_parameters.find( "allowed_methods" ) == location->_parameters.end() )
		return ( method == "GET" || method == "POST" || method == "DELETE" );
	else
	{
		std::vector< std::string > :: iterator it = location->_parameters[ "allowed_methods" ].begin();

		while ( it != location->_parameters[ "allowed_methods" ].end() )
		{
			if ( *it == method )
				return ( true );
			it++;
		}
	}

	return ( false );
}
