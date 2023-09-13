/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   listen_loop.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 14:42:00 by tda-silv          #+#    #+#             */
/*   Updated: 2023/09/13 21:28:21 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

void Gateway::listen_loop( char **env )	// ! throw possible
{
	int	ret;

	std::cout << COLOR_GREEN << "\n+++++++ Waiting for new connection ++++++++\n" << COLOR_RESET << std::endl;

	while (1)
	{
		ret = poll(poll_struct.data(), static_cast<nfds_t>(poll_struct.size() ), 0);	// poll() vérifie l'état de chaque socket
		if (ret == -1)
			my_perror_and_throw("poll error", std::exception() );

		check_new_connections();
		receive_on_connections();
		_send_responses( env );
		close_connections();

		if (siginit_status)
			break ;
	}
}
