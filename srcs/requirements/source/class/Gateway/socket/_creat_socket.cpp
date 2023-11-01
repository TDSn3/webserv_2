/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _creat_socket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 14:48:34 by tda-silv          #+#    #+#             */
/*   Updated: 2023/08/23 12:56:34 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

/* ************************************************************************** */
/*                                                                            */
/*   AF_INET		: IPv4.													  */
/*   SOCK_STREAM	: flux d'octets full-duplex.							  */
/*                                                                            */
/* ************************************************************************** */
int	Gateway::_creat_socket(void)
{
	int	fd;

	fd = socket(AF_INET, SOCK_STREAM /*| SOCK_CLOEXEC*/, 0);
	int value = 1;
	if (fd < 0 || setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value)) == -1) 
	{
		throw std::exception();
	}
	return (fd);
}
