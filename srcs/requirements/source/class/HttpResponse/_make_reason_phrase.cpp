/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _make_reason_phrase.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 19:21:54 by tda-silv          #+#    #+#             */
/*   Updated: 2023/09/17 12:45:06 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

void HttpResponse::_make_reason_phrase( void )
{
	if ( status_line.code == 200 )
		status_line.reason_phrase = "OK";
	else if ( status_line.code == 400 )
		status_line.reason_phrase = "Bad Request";
	else if ( status_line.code == 404 )
		status_line.reason_phrase = "Not Found";
	else if ( status_line.code == 405 )
		status_line.reason_phrase = "Method Not Allowed";
	else if ( status_line.code == 500 )
		status_line.reason_phrase = "Internal Server Error";
}
