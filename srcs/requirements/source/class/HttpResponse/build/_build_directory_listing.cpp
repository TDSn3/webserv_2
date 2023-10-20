/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _build_directory_listing.cpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfoucade <yfoucade@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 17:43:34 by yfoucade          #+#    #+#             */
/*   Updated: 2023/10/21 00:34:54 by yfoucade         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.hpp"

void	HttpResponse::_build_directory_listing( std::string dirname, std::string target_resource )
{
	std::vector< struct dirent >			entries = get_entries(dirname);
	std::vector< std::string >				html_lines = get_html_lines( dirname, entries, target_resource );
	std::vector< std::string >::iterator	it = html_lines.begin();
	std::vector< std::string >::iterator	end = html_lines.end();

	(void)dirname;
	str_body = "<html>";
	str_body += "<head><title>Index of " + target_resource + "</title></head>";
	str_body += "<body><h1>Index of " + target_resource + "</h1><hr><pre>";
	str_body += "<a href=\"../\">../</a>\n";
	// Add one <a> per file here
	for ( ; it != end; ++it )
		str_body += *it;
	str_body += "</pre><hr></body></html>";
	_set_status_line( 200, "OK" );
}