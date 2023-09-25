/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 11:00:08 by tda-silv          #+#    #+#             */
/*   Updated: 2023/09/22 19:03:54 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

void	HttpResponse::build( Request &request, char **env )	// ! throw possible
{	
	if ( request.get_final_status() == bad_request )
		my_perror_and_throw( "bad request", StatusCode( 400 ) );

	if ( request.request_line.method == "GET" )
	{
		if ( _get_method( request, env ) == true )			// ! throw possible
			return ;										// Le CGI a rempli la réponse
	}
	else if ( request.request_line.method == "POST" )
	{
		if ( _post_method( request, env ) == true )			// ! throw possible
			return ;										// TODO: add parsing on body
	}
	else if ( request.request_line.method == "DELETE" )
		_delete_method( request );							// ! throw possible
	else
		my_perror_and_throw( "method not allowed", StatusCode( 405 ) );

	_make_response( request );								// ! throw possible
};
