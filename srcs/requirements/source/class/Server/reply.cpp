/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reply.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 21:00:45 by tda-silv          #+#    #+#             */
/*   Updated: 2023/10/07 14:39:32 by tda-silv         ###   ########.fr       */
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
		connection.response.clear();
		connection.flush_request();
		connection.response_status = false;
	}
	std::cout << COLOR_RESET << std::endl;
}
