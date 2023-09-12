/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   listen_loop.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 14:42:00 by tda-silv          #+#    #+#             */
/*   Updated: 2023/09/12 14:42:36 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

void Gateway::listen_loop( char **env )
{
	(void) env;

	while (true)
	{	
		int	ret;
				
		// reset_fds();

		ret = poll(poll_struct.data(), static_cast<nfds_t>(poll_struct.size() ), 0);	// poll() vérifie l'état de chaque socket
		if (ret == -1)
			perror("poll error");	// TODO: a gerer plus tard
		
		// select(get_max_socket_fd() + 1, &_readfds, &_writefds, NULL, NULL);


		// open_new_connections();
		receive_on_connections();
		// send_responses();
		// close_connections();
	}
}
