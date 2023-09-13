/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Gateway.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 15:52:49 by yfoucade          #+#    #+#             */
/*   Updated: 2023/09/13 10:00:21 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <netdb.h>
#include <set>
#include <string>
#include <vector>
#include <poll.h>

#include "Connection.hpp"
#include "exceptions.hpp"
#include "Origin.hpp"
#include "Server.hpp"

class Gateway
{
	public:

		Gateway( std::string config_file );
		~Gateway();

		std::vector<pollfd>			poll_struct;

		void						create_servers( std::vector< std::string > content );
		void						create_origin_sockets_mapping( void );
		struct addrinfo*			resolve_name( const Origin& );
		void 						print_origin_sockets_mapping( void );
		void						listen_loop( char **env );
		void						add_fd_poll_struct(int fd, short events);
		// void	process_request( int socket, Origin origin );

	private:

		typedef std::vector< Server >::iterator			server_iter_type;
		typedef std::map< Origin, pollfd >::iterator	socket_iter_type;
		typedef std::vector< Connection >::iterator		connection_iter_type;

		bool						_fatal_error;
		std::vector< Server >		_servers;
		std::map< Origin, pollfd >	_map_origin_socket;
		std::vector< Connection >	_connections;
		fd_set						_readfds;
		fd_set						_writefds;

		Gateway&					operator=( const Gateway& );
		void						add_server( std::vector< std::string >::iterator, std::vector< std::string >::iterator );
		void						check_new_connections( void );
		void						open_connection( Origin origin, pollfd pfd );
		void						receive_on_connections( void );
		void						send_responses( void );
		void						close_connections( void );
		void						reply( Connection& );
		server_iter_type			decide_server( Connection& );
		int							_give_new_socket( const Origin& origin, short events );
		int							_creat_socket( void );
		void						_assign_socket_name( const Origin &origin, int &fd, struct sockaddr_in &address );
		int							_set_non_blocking_fd( int fd );
		void						_add_fd_poll_struct(int fd, short events);
};
