/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_html_lines.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfoucade <yfoucade@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 23:37:08 by yfoucade          #+#    #+#             */
/*   Updated: 2023/10/21 00:48:42 by yfoucade         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.hpp"

std::string get_html_line( std::string dirname, struct dirent entry, std::string target_resource )
{
	(void)dirname;
	std::string res = "<a href=\"" + target_resource.substr(1) + "/" + std::string(entry.d_name) + "\">";
	res += std::string(entry.d_name);
	if ( is_directory(dirname + "/" + std::string(entry.d_name)) )
		res += "/";
	res += "</a>\n";
	// TODO: add time and size ?
	// res += " 20-Oct-2023 17:50 0 \n";
	return res;
}

std::vector< std::string > get_html_lines( std::string dirname, std::vector< struct dirent > entries, std::string target_resource )
{
	std::vector< std::string > res;
	std::vector< struct dirent >::iterator it = entries.begin();
	std::vector< struct dirent >::iterator end = entries.end();

	for ( ; it != end; ++it )
	{
		if ( std::string(it->d_name) != "." && std::string(it->d_name) != ".." )
			res.push_back( get_html_line(dirname, *it, target_resource) );
	}
	return res;
}