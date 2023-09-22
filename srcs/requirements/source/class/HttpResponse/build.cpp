/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 11:00:08 by tda-silv          #+#    #+#             */
/*   Updated: 2023/09/22 16:00:33 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

void	HttpResponse::build( Request &request, char **env )	// ! throw possible
{	
	if ( request.get_final_status() == bad_request )
		my_perror_and_throw( "bad request", StatusCode( 400 ) );

	if (request.request_line.method == "GET")
		_set_status_line( 200, "OK" );	// ! throw possible
	else if (request.request_line.method == "POST")
		_post_method( request );		// ! throw possible
	else if (request.request_line.method == "DELETE")
		_delete_method( request );		// ! throw possible
	else
		my_perror_and_throw( "method not allowed", StatusCode( 405 ) );

	_make_response(request, env);		// ! throw possible
};
