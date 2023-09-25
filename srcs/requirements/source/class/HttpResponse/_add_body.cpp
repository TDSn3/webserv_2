/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _add_body.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 19:34:49 by tda-silv          #+#    #+#             */
/*   Updated: 2023/09/22 17:50:00 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

void	HttpResponse::_add_body( std::string path )	// Utilisé pendant une erreur
{
	std::ostringstream	oss;

	str_response += "Content-Length: ";

	try
	{
		str_body = _read_file_in_str( path );
	}
	catch ( const StatusCode &e )
	{
		str_body.clear();
		str_body += "<!DOCTYPE html>\r\n";
		str_body += "\r\n";
		str_body += "<html>\r\n";
		str_body += "\t<head>\r\n";
		str_body += "\t\t<title>404 Not Found</title>\r\n";
		str_body += "\t</head>\r\n";
		str_body += "\t<body bgcolor=\"white\">\r\n";
		str_body += "\t\t<center><h1>404 Not Found</h1></center>\r\n";
		str_body += "\t\t<hr><center>webserv</center>\r\n";
		str_body += "\t</body>\r\n";
		str_body += "<html>\r\n";
	}

	oss << str_body.size();
	str_response += oss.str();

	str_response += "\r\n";
	str_response += "\r\n";
	str_response += str_body;
}