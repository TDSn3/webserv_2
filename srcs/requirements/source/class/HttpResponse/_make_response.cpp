/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _make_response.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 09:52:55 by tda-silv          #+#    #+#             */
/*   Updated: 2023/09/17 11:14:24 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

void	HttpResponse::_make_response(Request &request, char **env)	// ! throw possible
{
	std::ostringstream	oss;

	(void) request;

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
