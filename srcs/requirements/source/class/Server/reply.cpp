/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reply.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 21:00:45 by tda-silv          #+#    #+#             */
/*   Updated: 2023/09/13 21:04:46 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

void	Server::reply( Connection &connection, char **env )
{
	connection.response.build(connection.get_request(), env);
	
	size_t	to_send =  connection.response.str_response.size();
	ssize_t	sended;

	std::cout << "Server #" << _id << " responds to connection.\n";			
	std::cout << "[" << COLOR_BOLD_GREEN << "RESPONSE" << COLOR_RESET << "]\n" << connection.response.str_response << std::endl;
	std::cout << "[" << COLOR_BOLD_RED << "END OF RESPONSE" << COLOR_RESET << "]\n" << std::endl;

	sended = send(connection.get_socket() , connection.response.str_response.c_str() , to_send, 0);
	if (sended < static_cast<ssize_t>(to_send) )
	{
		std::cout << COLOR_BOLD_RED << "RESPONSE WAS NOT COMPLETELY SEND !" << std::endl;	// TODO: gerer cela
	}
	connection.response.clear();	
	connection.flush_request();
}
