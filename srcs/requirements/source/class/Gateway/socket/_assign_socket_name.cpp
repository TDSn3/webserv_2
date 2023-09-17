/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _assign_socket_name.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 15:03:33 by tda-silv          #+#    #+#             */
/*   Updated: 2023/08/23 12:56:18 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

/* ************************************************************************** */
/*                                                                            */
/*   htonl()	: convertit un entier  non  signé (par exemple une adresse)	  */
/*				  en une représentation réseau.								  */
/*   htons()	: convertit un entier court non  signé (par exemple port)	  */
/*				  en une représentation réseau.								  */
/*                                                                            */
/* ************************************************************************** */
void	Gateway::_assign_socket_name(const Origin& origin, int &fd, struct sockaddr_in &address)
{
	std::stringstream	ss(origin.get_port() );
	int					port;

	ss >> port;
	memset( (char *) &address, 0, sizeof(address) );
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons(static_cast<uint16_t>(port) );

	if (bind(fd, (struct sockaddr *) &address, sizeof(address) ) < 0) 
	{
		perror("bind");
		// TODO: err plus tard
	}
}
