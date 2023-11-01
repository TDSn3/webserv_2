/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _make_response.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 09:52:55 by tda-silv          #+#    #+#             */
/*   Updated: 2023/11/01 08:07:18 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

static std::string to_lower_str( std::string str );

void	HttpResponse::_make_response( Request &request, std::string &path )	// ! throw possible
{

/* ************************************************************************** */
/*                                                                            */
/*   status line															  */
/*                                                                            */
/* ************************************************************************** */

	std::ostringstream	oss;

	oss << status_line.code;

	str_response += status_line.version;		// HTTP version
	str_response += " ";
	str_response += oss.str();					// status code
	str_response += " ";
	str_response += status_line.reason_phrase;	// reason_phrase
	str_response += "\r\n";

/* ************************************************************************** */
/*                                                                            */
/*   header																	  */
/*                                                                            */
/* ************************************************************************** */

	oss.str("");
	oss.clear();
	oss << str_body.size();

	_add_content_type( path );



	std::map< std::string, std::vector< std::string > > :: iterator	it = request._header_section.begin();

	while ( it != request._header_section.end() )
	{		
		if ( to_lower_str( it->first ) == "connection" )
			if ( it->second.empty() == false && to_lower_str( *( it->second.begin() ) ) == "close" )
				_add_field_line( "connection", "close" );
		it++;
	}



	_add_field_line( "content-length", oss.str() );
	str_response += "\r\n";

/* ************************************************************************** */
/*                                                                            */
/*   body																	  */
/*                                                                            */
/* ************************************************************************** */

	str_response += str_body;
}

static std::string to_lower_str( std::string str )
{
	std::string	ret;

	for ( size_t i = 0; str[i]; i++ )
		ret += std::tolower( static_cast<unsigned char> ( str[i] ) ) ;
	return ( ret );
}
