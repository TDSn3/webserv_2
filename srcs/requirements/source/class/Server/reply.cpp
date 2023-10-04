/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reply.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 21:00:45 by tda-silv          #+#    #+#             */
/*   Updated: 2023/10/04 21:07:10 by tda-silv         ###   ########.fr       */
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

	std::cout << "[" << COLOR_BOLD_GREEN << "RESPONSE" << COLOR_RESET << "]\n" << std::endl;
	// std::cout << connection.response.str_response << std::endl;
	std::cout << "[" << COLOR_BOLD_RED << "END OF RESPONSE" << COLOR_RESET << "]\n" << std::endl;

	if ( to_send )
	{
		sent = send( connection.get_socket() , connection.response.str_response.c_str() + total_sent , to_send, 0 );

		if ( sent <  static_cast< ssize_t >( to_send ) )
			std::cout << COLOR_BOLD_RED << "RESPONSE WAS NOT COMPLETELY SEND !" << COLOR_RESET << std::endl;

   		total_sent += sent;
   		to_send -= sent;

		std::cout << COLOR_BOLD_RED << "sent : " << sent << std::endl;
		std::cout << COLOR_BOLD_RED << "total_sent : " << total_sent << std::endl;
		std::cout << COLOR_BOLD_RED << "to_send : " << to_send << std::endl;
	}
	if ( to_send == 0 )
	{
		std::cout << COLOR_BOLD_GREEN << "THE RESPONSE WAS COMPLETELY SEND." << COLOR_RESET << std::endl;
		connection.response.clear();
		connection.flush_request();
		connection.response_status = false;
	}
	if ( exit_now == 2)
		exit (1);
}