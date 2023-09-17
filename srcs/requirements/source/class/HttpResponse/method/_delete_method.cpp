/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _delete_method.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 14:08:02 by tda-silv          #+#    #+#             */
/*   Updated: 2023/09/17 14:27:29 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

void	HttpResponse::_delete_method( Request &request )	// ! throw possible
{
	std::string	path;
	std::string	new_path;

	path = request.request_line.parsed_url.path;
	if ( !path.empty() && path[0] == '/' )
		new_path = std::string( ROOT ) + path;
	else if ( !path.empty() && path[0] != '/' )
		new_path = std::string( ROOT ) + "/" + path;

	if ( std::remove( new_path.c_str() ) == 0 )
		_set_status_line( 200, "OK" );
	else
		my_perror_and_throw( "DELETE: internal server error", StatusCode(500) );
}
