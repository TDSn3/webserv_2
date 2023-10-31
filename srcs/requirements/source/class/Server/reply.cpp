/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reply.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfoucade <yfoucade@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 21:00:45 by tda-silv          #+#    #+#             */
/*   Updated: 2023/10/31 21:52:07 by yfoucade         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

void	Server::reply( Connection &connection, char **env )
{
	try
	{
		connection.get_request().print_request();
		if ( connection.response_status == false )
		{
			if ( connection.get_request().get_parsing_status() == expect_100_continue )
				connection.response.build_100_continue( connection.get_request(), *this );
			else
				connection.response.build( connection.get_request(), env, *this );	// Si build throw, build une réponse d'erreur
			connection.response_status = true;
		}
	}
	catch( const StatusCode &e )
	{
		e.build_response_error( connection );
	}

	size_t	&to_send = connection.response.to_send;
	ssize_t	&sent = connection.response.sent;
	size_t	&total_sent = connection.response.total_sent;

	std::cout << "[" << COLOR_BOLD_GREEN << "RESPONSE" << COLOR_RESET << "] ";
	if ( total_sent == 0)
	{	
		std::cout << std::endl;
		std::cout << COLOR_DIM << ( connection.response.str_response ).substr( 0, 200);
		if ( ( connection.response.str_response ).size() > 200 )
			std::cout << COLOR_DIM_RED << " ... " << COLOR_RESET << COLOR_DIM << *( ( connection.response.str_response ).end() - 1 );
		std::cout << COLOR_RESET << std::endl;
		std::cout << "[" << COLOR_BOLD_RED << "END OF RESPONSE" << COLOR_RESET << "] ";
	}

	if ( to_send )
	{
		sent = send( connection.get_socket() , connection.response.str_response.c_str() + total_sent , to_send, 0 );

   		total_sent += sent;
   		to_send -= sent;

		std::cout << COLOR_DIM_RED << "sent : " << sent << "; ";
		std::cout << COLOR_DIM_RED << "total_sent : " << total_sent << "; ";
		std::cout << COLOR_DIM_RED << "to_send : " << to_send << "; ";
		if ( sent <  static_cast< ssize_t >( to_send ) )
			std::cout << COLOR_DIM << COLOR_BOLD_RED << "RESPONSE WAS NOT COMPLETELY SEND ! " << COLOR_RESET;
	}
	if ( to_send == 0 )
	{
		std::cout << COLOR_DIM << COLOR_BOLD_GREEN << "THE RESPONSE WAS COMPLETELY SEND" << COLOR_RESET;
		/*
		The line named "line A" was there before we handled the 100 Continue status code
		Now we do not want to systematically flush the request.
		We want to call flush_request() only if a final response has been sent.
		( (complete request) or (100-continue and error) )
		Otherwise, we let the request decide on its next status (body or chunk).
		*/
		if ( connection.get_request().get_parsing_status() == expect_100_continue )
			connection.get_request().set_status_after_continue();
		else
			connection.flush_request(); // line A
			
		connection.response_status = false;
		if ( connection.response.status_line.code == 400 )
			connection.set_close(true);
		connection.response.clear();
	}
	std::cout << COLOR_RESET << std::endl;
}
