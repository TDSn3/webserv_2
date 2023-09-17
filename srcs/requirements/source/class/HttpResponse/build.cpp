/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 11:00:08 by tda-silv          #+#    #+#             */
/*   Updated: 2023/09/17 15:18:48 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

void	HttpResponse::build( Request &request, char **env )	// ! throw possible
{	
	if (request.request_line.method == "GET")
	{
		_set_status_line( 200, "OK" );
	}
	else if (request.request_line.method == "POST")
	{
		_post_method( request );
	}
	else if (request.request_line.method == "DELETE")
		_delete_method( request );
	else
		my_perror_and_throw( "cgi file does not exist", StatusCode( 405 ) );

	_make_response(request, env);	// ! throw possible
};
