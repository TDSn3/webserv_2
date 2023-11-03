/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   listen_loop.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 10:24:15 by tda-silv          #+#    #+#             */
/*   Updated: 2023/11/03 11:54:41 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

void	listen_loop( Gateway &gateway, char **env )	// ! throw possible
{
	int					ret;
	std::vector<pollfd>	&poll_struct = gateway.poll_struct;

	std::cout << COLOR_GREEN << "\n+++++++ Waiting for new connection ++++++++\n" << COLOR_RESET << std::endl;

	while (1)
	{
		ret = poll(poll_struct.data(), static_cast<nfds_t>(poll_struct.size() ), 0);
		if (ret == -1)
			my_perror_and_throw("poll error", std::exception() );

		gateway.check_new_connections();
		gateway.receive_on_connections();
		gateway.send_responses( gateway, env );
		gateway.close_connections();

		if (siginit_status)
			break ;
	}
}
