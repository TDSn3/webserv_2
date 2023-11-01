/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _get_method.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 17:32:47 by tda-silv          #+#    #+#             */
/*   Updated: 2023/11/01 13:09:21 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

bool	HttpResponse::_get_method( Request &request, char **env, std::string &path, Server &server )	// ! throw possible
{
	(void) request;
	(void) env;
	(void) server;
	str_body = _read_file_in_str( path );	// ! throw possible
	_set_status_line( 200, "OK" );
	return ( false );
}
