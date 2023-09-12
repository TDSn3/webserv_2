/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Gateway.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 16:30:24 by yfoucade          #+#    #+#             */
/*   Updated: 2023/09/12 14:42:07 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

Gateway::Gateway( std::string config_file ) : _fatal_error(false)
{
	std::vector< std::string > content = read_file(config_file);
	create_servers(content);
	
	create_origin_sockets_mapping();
	print_origin_sockets_mapping();
}

Gateway::~Gateway( void )
{
}

void	Gateway::create_servers( std::vector< std::string > content )
{
	std::vector< std::string >::iterator server_block_start = content.begin();
	std::vector< std::string >::iterator server_block_end = content.end();
	std::vector< std::string >::iterator end = content.end();

	for ( ; server_block_start != end; ++server_block_start )
	{

		if ( server_block_start->compare("server {") )	// TODO: tokenize possible
			continue;

		for ( server_block_end = server_block_start + 1;
			  server_block_end->compare("}") && (server_block_end != end);
			  ++server_block_end){}

		std::cout << COLOR_BOLD_RED << "test 1\n" << COLOR_RESET;

		if ( server_block_end != end )
			add_server(server_block_start, server_block_end);

		server_block_start = server_block_end;
	}

	std::cout << COLOR_BOLD_RED << "test end create_servers\n" << COLOR_RESET;
}

void Gateway::add_server( std::vector< std::string >::iterator server_block_start, std::vector< std::string >::iterator server_block_end)
{
	try
	{
		_servers.push_back(Server(server_block_start, server_block_end));
	}
	catch( const std::exception& e)
	{
		std::cerr << "exception:\n";
		std::string wot = e.what();
		std::cerr << wot << std::endl;
	}
}

void	Gateway::create_origin_sockets_mapping( void )
{
	std::vector< Server >::iterator		servers_it = _servers.begin();
	std::vector< Server >::iterator		servers_end = _servers.end();
	std::set< Origin >::iterator		origin_it;
	std::set< Origin >::iterator		origin_end;
	std::map< Origin, int >::iterator	orig_socket_it;

	for ( ; servers_it != servers_end; ++servers_it)
	{
		origin_it = servers_it->get_origins().begin();
		origin_end = servers_it->get_origins().end();
		for ( ; origin_it != origin_end; ++origin_it )
		{
			if ( _map_origin_socket.count(*origin_it) )
				continue;
			// TODO: handle errors (recursively) in the following lines
			int new_socket = _give_new_socket(*origin_it, POLLIN | POLLOUT);	// TODO: a affiner plus tard
			_map_origin_socket.insert(std::make_pair(*origin_it, new_socket));
		}
	}
}

struct addrinfo* Gateway::resolve_name( const Origin& origin )
{
	struct addrinfo*	res;
	struct addrinfo		hints;
	int					err;

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// TODO: handle error
	err = getaddrinfo(origin.get_host().c_str(), origin.get_port().c_str(), &hints, &res);
	if ( err )
	{
		std::cerr << "Error\n";
	}
	return res;
}

void Gateway::reset_fds( void )
{
	std::map< Origin, int >::iterator socket_it = _map_origin_socket.begin();
	std::map< Origin, int >::iterator socket_end = _map_origin_socket.end();
	std::vector< Connection >::iterator connection_it = _connections.begin();
	std::vector< Connection >::iterator connection_end = _connections.end();
	int socket;

	FD_ZERO(&_readfds);
	FD_ZERO(&_writefds);
	for ( ; socket_it != socket_end; ++socket_it)
		FD_SET(socket_it->second, &_readfds);
	for ( ; connection_it != connection_end; ++connection_it )
	{
		socket = connection_it->get_socket();
		FD_SET(socket, &_readfds);
		FD_SET(socket, &_writefds);
	}
}

void	Gateway::open_new_connections( void )
{
	socket_iter_type socket_iter = _map_origin_socket.begin();
	socket_iter_type end = _map_origin_socket.end();
	// Open new_connections
	for ( ; socket_iter != end; ++socket_iter )
	{
		if ( FD_ISSET(socket_iter->second, &_readfds) )
			open_connection( socket_iter->first, socket_iter->second);
	}
}

void	Gateway::open_connection( Origin origin, int socket )
{
	// todo: dup() and do not add to _connections if initialization failed
	int new_socket = accept( socket, NULL, NULL );
	std::cout << "Accepted new connection on ";
	std::cout << origin.get_host() << ":" << origin.get_port() << ". ";
	std::cout << "new socket on fd: " << new_socket << std::endl;
	_connections.push_back( Connection(origin, new_socket) );
}

void	Gateway::receive_on_connections( void )
{
	// connection_iter_type connection_iter = _connections.begin();
	// connection_iter_type end = _connections.end();

	std::vector<pollfd>	:: iterator	it = poll_struct.begin();

	while (it != poll_struct.end())
	{
		if (it->revents & (POLLIN | POLLERR | POLLHUP) )
			std::cout << "POLLIN" << std::endl;
		if (it->revents & POLLOUT )
			std::cout << "POLLOUT" << std::endl;

		it++;
	}

	// Receive messages on connections
	// for ( ; connection_iter != end; ++connection_iter )
	// {
	// 	if ( FD_ISSET(connection_iter->get_socket(), &_readfds) )
	// 		connection_iter->receive();
	// }
}

void	Gateway::send_responses( void )
{
	connection_iter_type connection_iter = _connections.begin();
	connection_iter_type end = _connections.end();
	server_iter_type	server_iter;

	// If a connection is ready to answer and can be written on, send response.
	for ( ; connection_iter != end; ++connection_iter )
	{
		if ( connection_iter->is_ready_for_reply() && FD_ISSET(connection_iter->get_socket(), &_writefds) )
		{
			std::cout << "send_responses: host = " << connection_iter->get_request().get_host_value() << std::endl;
			server_iter = decide_server( *connection_iter );
			server_iter->reply( *connection_iter );
			// reply(*connection_iter); // sets the 'close' flag to true if applicable
		}
	}
}

typename Gateway::server_iter_type
Gateway::decide_server( Connection& connection )
{
	server_iter_type	it = _servers.begin();
	server_iter_type	end = _servers.end();
	server_iter_type	res = end;

	std::cout << "decide_server: host = " << connection.get_request().get_host_value() << std::endl;
	for ( ; it != end; ++it )
	{
		// if origin mismatch, continue
		if ( !it->listens_to_origin(connection.get_origin()) )
			continue;
		// if host match -> return or break
		if ( it->has_server_name(connection.get_request().get_host_value()) )
		{
			res = it;
			break;
		}
		// if res == end, it is the default server, set res = it.
		if ( res == end )
		{
			res = it;
			std::cout << "decide_server: Server #" << res->get_id() << " is default\n";
		}
	}
	return res;
}

void	Gateway::close_connections( void )
{
	connection_iter_type connection_iter = _connections.begin();
	connection_iter_type end = _connections.end();

	while ( connection_iter != end )
	{
		connection_iter->update_close();
		if ( connection_iter->get_close() )
		{
			connection_iter->close_connection();
			connection_iter = _connections.erase(connection_iter);
		}
		else
			++connection_iter;
	}
}

int		Gateway::get_max_socket_fd( void )
{
	int res = -1;
	std::map< Origin, int >::iterator socket_it = _map_origin_socket.begin();
	std::map< Origin, int >::iterator socket_end = _map_origin_socket.end();
	std::vector< Connection >::iterator connection_it = _connections.begin();
	std::vector< Connection >::iterator connection_end = _connections.end();

	for (; socket_it != socket_end; ++socket_it)
		res = std::max(res, socket_it->second);
	for (; connection_it != connection_end; ++connection_it)
		res = std::max(res, connection_it->get_socket());
	return res;
}

// void	Gateway::process_request( int socket, Origin origin )
// {
// 	char buff[1024];
// 	int ret;
// 	std::vector< std::string > request;

// 	std::cout << "Processing request on origin: ";
// 	std::cout << origin.get_host() << ":" << origin.get_port();
// 	std::cout << std::endl;
// 	while ( (ret = recv(socket, buff, 1023, MSG_DONTWAIT)) != -1)
// 	{
// 		buff[ret] = '\0';
// 		request.push_back(buff);
// 		std::cout << request.size() << std::endl;
// 		send(socket, buff, ret, 0);
// 	}
// 	std::cout << request[1] << std::endl;
// }

void 	Gateway::print_origin_sockets_mapping( void )
{
	std::map< Origin, int >::iterator it = _map_origin_socket.begin();
	std::map< Origin, int >::iterator end = _map_origin_socket.end();

	for ( ; it != end; ++it )
	{
		std::cout << it->first.get_host() << ":" << COLOR_BLUE << it->first.get_port() << COLOR_RESET;
		std::cout << " has socket number " << COLOR_GREEN << it->second << COLOR_RESET;
		std::cout << std::endl;
		std::cout << COLOR_BOLD_RED << "test loop\n" << COLOR_RESET;
	}
	std::cout << COLOR_BOLD_RED << "test end print_origin_sockets_mapping\n" << COLOR_RESET;
}

void	Gateway::reply( Connection& connection )
{
	(void)connection;
	// Origin origin = connection.get_origin();
	// std::cout << "Replying on origin: " << origin.get_host();
	// std::cout << ":" << origin.get_port() << std::endl;
}
