/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_new_connections.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 10:30:53 by tda-silv          #+#    #+#             */
/*   Updated: 2023/09/17 10:31:49 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

void	Gateway::check_new_connections( void )
{
	socket_iter_type	socket_iter = _map_origin_socket.begin();
	size_t				i;

	i = 0;
	while ( i < poll_struct.size() && i < _map_origin_socket.size() )
	{
		if ( poll_struct[i].revents & POLLIN )
			open_connection( socket_iter->first, poll_struct[i] );

		i++;
	}
}
