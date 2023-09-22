/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _make_response.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 09:52:55 by tda-silv          #+#    #+#             */
/*   Updated: 2023/09/22 16:41:26 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

static void	_read_body( Request &request );

void	HttpResponse::_make_response( Request &request, char **env )	// ! throw possible
{
	_read_body( request );

/* ************************************************************************** */

	std::ostringstream	oss;

	oss << status_line.code;

	str_response += status_line.version;
	str_response += " ";
	str_response += oss.str();
	str_response += " ";
	str_response += status_line.reason_phrase;
	str_response += "\r\n";

	_give_content_type(request);

	_add_body(request, env);		// ! throw possible
}

static void	_read_body( Request &request )
{
	std::cout << COLOR_MAGENTA;

	std::cout << "content_length " << COLOR_BOLD_MAGENTA;
	
	request.get_content_length_status() == true ? ( std::cout << "YES" ) : ( std::cout << "NO" );

	std::cout << "\n" << COLOR_RESET;

	std::cout << "\n";
}
