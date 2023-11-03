/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIResponse.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfoucade <yfoucade@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 12:34:55 by yfoucade          #+#    #+#             */
/*   Updated: 2023/11/02 13:44:09 by yfoucade         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.hpp"

CGIResponse::CGIResponse( std::string raw_cgi_response ):
status_line( "HTTP/1.1 200 OK" ),
raw_data( raw_cgi_response ),
index(0)
{
	std::string line = get_next_line();

	while ( line.size() > 1)
	{
		if ( ft_tolower_str( line.substr(0, 7) ) == "status:" )
			status_line = "HTTP/1.1" + line.substr(7);
		headers.push_back(line);
		line = get_next_line();
	}
	body = raw_cgi_response.substr( index );
	
	std::ostringstream	oss;
	oss << body.size();
	headers.push_back( "Content-Length: " + oss.str() );
	
	parsed_response = status_line + "\n";
	for ( size_t i = 0; i < headers.size(); ++i )
		parsed_response += headers[i] + "\n";
	parsed_response += "\n" + body;
}

CGIResponse::~CGIResponse(){}

std::string	CGIResponse::get_next_line( void )
{
	std::string	ret;
	size_t		index_of_newline = raw_data.find('\n', index);
	
	ret = raw_data.substr(index, index_of_newline - index);
	index = index_of_newline + 1;
	return ret;
}
