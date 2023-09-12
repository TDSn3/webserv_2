/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _give_new_socket.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 13:44:42 by tda-silv          #+#    #+#             */
/*   Updated: 2023/09/12 14:37:13 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

int	Gateway::_give_new_socket( const Origin& origin, short events )
{
	struct sockaddr_in	address;
	int					new_socket;

	new_socket = _creat_socket();						// Crée un socket pour ce connecter au serveur	// ! throw possible
	_assign_socket_name(origin, new_socket, address);	// Affecte un "nom" au socket crée				// ! throw possible

	if (listen(new_socket, 3) < 0)					// Prépare le socket pour la connexion
	{
		// TODO: err plus tard
	}

	_set_non_blocking_fd(new_socket);
	add_fd_poll_struct(new_socket, events);		// ajoute le socket à la struct de poll

	// struct addrinfo	*origin_info = resolve_name(origin);
	// // int new_socket = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
	// int new_socket = socket(AF_INET, SOCK_STREAM | SOCK_CLOEXEC, IPPROTO_TCP);
	// int value = 1;
	// setsockopt(new_socket, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value));
	// int err = bind(new_socket, origin_info->ai_addr, origin_info->ai_addrlen);
	// if ( err )
	// {
	// 	std::cout << "failed to bind socket to origin ";
	// 	std::cout << origin.get_host() << ":" << origin.get_port() << std::endl;
	// }
	// freeaddrinfo(origin_info);
	// err = listen(new_socket, 2);		// TODO: gestion err
	// _set_non_blocking_fd(new_socket);
	return (new_socket);
}
