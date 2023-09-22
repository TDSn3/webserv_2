/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _set_non_blocking_fd.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 13:20:00 by tda-silv          #+#    #+#             */
/*   Updated: 2023/09/22 16:06:27 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

int	Gateway::_set_non_blocking_fd(int fd)
{
	if ( fcntl(fd, F_SETFL, O_NONBLOCK) )	// unique utilisation autorisé par le sujet
	{
		close(fd);
		// my_perror_and_throw("fcntl", std::exception() );
	}
	return (0);
}
