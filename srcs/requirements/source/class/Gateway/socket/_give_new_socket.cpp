/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _give_new_socket.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfoucade <yfoucade@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 13:44:42 by tda-silv          #+#    #+#             */
/*   Updated: 2023/10/18 06:26:02 by yfoucade         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

int	Gateway::_give_new_socket( const Origin& origin, short events )
{
	struct sockaddr_in	address;
	int					new_socket;

	new_socket = _creat_socket();						// Crée un socket pour ce connecter au serveur	// ! throw possible
	_assign_socket_name(origin, new_socket, address);	// Affecte un "nom" au socket crée				// ! throw possible

	if (listen(new_socket, 256) < 0)						// Prépare le socket pour la connexion
	{
		perror("listen");
		// TODO: err plus tard
	}

	_set_non_blocking_fd(new_socket);
	add_fd_poll_struct(new_socket, events);				// ajoute le socket à la struct de poll

	return (new_socket);
}
