/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _make_response.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 09:52:55 by tda-silv          #+#    #+#             */
/*   Updated: 2023/09/22 18:15:07 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

void	HttpResponse::_make_response( Request &request )	// ! throw possible
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

	_add_content_type( request );
	_add_field_line( "content-length", oss.str() );
	str_response += "\r\n";

/* ************************************************************************** */
/*                                                                            */
/*   body																	  */
/*                                                                            */
/* ************************************************************************** */

	str_response += "\r\n";
	str_response += str_body;
}
