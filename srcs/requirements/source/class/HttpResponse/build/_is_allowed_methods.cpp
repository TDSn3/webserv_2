/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _is_allowed_methods.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 10:49:16 by tda-silv          #+#    #+#             */
/*   Updated: 2023/10/02 10:50:32 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

bool	HttpResponse::_is_allowed_methods( Location *location, std::string method )
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
