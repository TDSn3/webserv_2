/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   receive_on_connections.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 10:33:10 by tda-silv          #+#    #+#             */
/*   Updated: 2023/09/22 15:46:33 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

void	Gateway::receive_on_connections( void )
{
	size_t	i = _map_origin_socket.size();

	while ( i < poll_struct.size() )
	{
		if ( poll_struct[i].revents & (POLLIN | POLLERR | POLLHUP) )
			_connections[ i - _map_origin_socket.size() ].receive();
		i++;
	}
}
 