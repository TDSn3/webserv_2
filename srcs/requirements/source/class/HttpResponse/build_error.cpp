/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_error.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 11:01:25 by tda-silv          #+#    #+#             */
/*   Updated: 2023/10/04 15:47:03 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

void	HttpResponse::build_error(Request &request, const int status_code)
{
	std::ostringstream	oss;
			
	(void) request;
	status_line.version = std::string(HTTP_VERSION);
	status_line.code = status_code;
	_make_reason_phrase();

	_add_status_line();

	_add_field_line("server", "webserv");
	_add_field_line("allow", "GET, POST, DELETE");
	_add_field_line("content-type", "text/html");
	_add_field_line("connection", "close");

	oss << status_code;
	_add_body("www/error_page/" + oss.str() + ".html");

	to_send =  str_response.size();
	sent = 0;
	total_sent = 0;
};