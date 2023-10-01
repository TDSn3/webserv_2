/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _delete_method.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 14:08:02 by tda-silv          #+#    #+#             */
/*   Updated: 2023/10/01 16:33:28 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

void	HttpResponse::_delete_method( std::string &path )	// ! throw possible
{
	if ( std::remove( path.c_str() ) == 0 )
		_set_status_line( 200, "OK" );
	else
		my_perror_and_throw( "HttpResponse::_delete_method: internal server error", StatusCode(500) );
}
