/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_fd_poll_struct.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/11 12:41:16 by tda-silv          #+#    #+#             */
/*   Updated: 2023/09/12 14:10:15 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

void	Gateway::add_fd_poll_struct(int fd, short events)
{
	pollfd	new_poll_struct;

	new_poll_struct.fd = fd; 
	new_poll_struct.events = events;
	new_poll_struct.revents = 0;
	poll_struct.push_back(new_poll_struct);
}
