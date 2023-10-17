/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _give_content_type.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 11:10:10 by tda-silv          #+#    #+#             */
/*   Updated: 2023/10/01 16:37:05 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

static std::string	give_uri_path_extension_name(std::string uri);

void	HttpResponse::_add_content_type( std::string &path )
{
	std::string	extension_name;

	str_response += "content-type: ";
	extension_name = give_uri_path_extension_name( path );
	if (extension_name == "html" || extension_name == "css")
		str_response += "text/" + extension_name;
	else if (extension_name == "javascript" || extension_name == "js")
		str_response += "application/javascript";
	else if (extension_name == "png")
		str_response += "image/png";
	str_response += "; charset=UTF-8\r\n";
}

static std::string	give_uri_path_extension_name(std::string uri)
{
	size_t		dot_pos;
	std::string	extension_name;

	dot_pos = uri.rfind('.');
	if (dot_pos == std::string::npos)
		return ("");

	extension_name = uri.substr(dot_pos + 1);
	if (extension_name == "cgi")
		extension_name = "html";

	if (extension_name != "html"
		&& extension_name != "css"
		&& extension_name != "javascript"
		&& extension_name != "js"
		&& extension_name != "ico"
		&& extension_name != "png"
		&& extension_name != "cgi")
		return ("");

	return (extension_name);
}
