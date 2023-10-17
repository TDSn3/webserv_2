/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _get_method.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 17:32:47 by tda-silv          #+#    #+#             */
/*   Updated: 2023/10/01 16:14:52 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

bool	HttpResponse::_get_method( Request &request, char **env, std::string &path )	// ! throw possible
{
	if ( path.find( "cgi-bin/" ) != std::string::npos )
	{
		std::cout << COLOR_BOLD_CYAN << "CGI detected" << COLOR_RESET << std::endl;
		str_response = _exec_cgi( path, request, env );			// ! throw possible
		return ( true );
	}
	else
		str_body = _read_file_in_str( path );					// ! throw possible
	_set_status_line( 200, "OK" );
	return ( false );
}
