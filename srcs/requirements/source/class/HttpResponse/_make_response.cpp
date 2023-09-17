/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _make_response.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 09:52:55 by tda-silv          #+#    #+#             */
/*   Updated: 2023/09/17 14:12:13 by tda-silv         ###   ########.fr       */
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
	if ( request.get_content_length_status() == true )
		std::cout << COLOR_YELLOW << "content_length OK\n" << COLOR_RESET;
	else
		std::cout << COLOR_YELLOW << "content_length ERROR\n" << COLOR_RESET;
}
