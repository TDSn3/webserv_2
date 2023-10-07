/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_responses.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 20:58:07 by tda-silv          #+#    #+#             */
/*   Updated: 2023/10/07 12:20:27 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

void	Gateway::send_responses( char **env )
{
	size_t				i = _map_origin_socket.size();
	server_iter_type	server_iter;

	while ( i < poll_struct.size() )
	{
		if ( poll_struct[i].revents & POLLOUT && _connections[ i - _map_origin_socket.size() ].is_ready_for_reply() == true )
		{
			server_iter = decide_server( _connections[ i - _map_origin_socket.size() ] );
			server_iter->reply( _connections[ i - _map_origin_socket.size() ], env );
		}
		i++;
	}
}
