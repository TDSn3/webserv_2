/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Gateway.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 15:52:49 by yfoucade          #+#    #+#             */
/*   Updated: 2023/11/03 11:42:36 by tda-silv         ###   ########.fr       */
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
#include "other/exceptions.hpp"
#include "Origin.hpp"
#include "Server.hpp"

class Gateway
{
	public:

		typedef std::vector< Server >::iterator			server_iter_type;
		typedef std::map< Origin, pollfd >::iterator	socket_iter_type;
		typedef std::vector< Connection >::iterator		connection_iter_type;

		Gateway( std::string config_file );
		~Gateway();

		std::vector<pollfd>			poll_struct;
		std::map< Origin, pollfd >	_map_origin_socket;

		void						create_servers( std::vector< std::string > content );
		void						create_origin_sockets_mapping( void );
		void 						print_origin_sockets_mapping( void );
		void						add_fd_poll_struct(int fd, short events);
		void						check_new_connections( void );
		void						receive_on_connections( void );
		void						send_responses( Gateway &gateway, char **env );
		void						close_connections( void );

	private:

		Gateway						&operator = ( const Gateway & );

		bool						_fatal_error;
		std::vector< Server >		_servers;
		std::vector< Connection >	_connections;
		fd_set						_readfds;
		fd_set						_writefds;

		void						add_server( std::vector< std::string >::iterator, std::vector< std::string >::iterator );
		void						open_connection( Origin origin, pollfd pfd );
		server_iter_type			decide_server( Connection& );
		int							_give_new_socket( const Origin& origin, short events );
		int							_creat_socket( void );
		void						_assign_socket_name( const Origin &origin, int &fd, struct sockaddr_in &address );
		int							_set_non_blocking_fd( int fd );
		void						_add_fd_poll_struct(int fd, short events);
};
