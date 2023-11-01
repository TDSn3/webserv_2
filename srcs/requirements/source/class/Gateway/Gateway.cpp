/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Gateway.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfoucade <yfoucade@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 16:30:24 by yfoucade          #+#    #+#             */
/*   Updated: 2023/10/31 13:21:15 by yfoucade         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

typedef std::vector< Server >::iterator	server_iter_type;

Gateway::Gateway( std::string config_file ) : _fatal_error(false)
{
	std::vector< std::string > content = read_file(config_file);
	create_servers(content);

	create_origin_sockets_mapping();
	print_origin_sockets_mapping();
}

Gateway::~Gateway( void )
{

	socket_iter_type	socket_iter = _map_origin_socket.begin();
	for ( ; socket_iter != _map_origin_socket.end(); socket_iter = _map_origin_socket.begin())
	{
		std::cout << "closing socket " << socket_iter->second.fd << std::endl;
		close(socket_iter->second.fd);
		_map_origin_socket.erase(socket_iter);
	}
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

		if ( server_block_end != end )
			add_server(server_block_start, server_block_end);

		server_block_start = server_block_end;
	}
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
			try {
				int new_socket = _give_new_socket(*origin_it, POLLIN);	// TODO: a affiner plus tard
				if (new_socket < 0)
					continue ;
			}
			catch ( std::exception &e ){
				std::cout << "_give_new_socket failed\n";
				continue;
			}
			_map_origin_socket.insert(std::make_pair(*origin_it, poll_struct.back() ) );
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

void	Gateway::open_connection( Origin origin, pollfd pfd )
{
	// todo: dup() and do not add to _connections if initialization failed
	int new_socket = accept( pfd.fd, NULL, NULL );	// TODO: peut etre ajouter address

	if (new_socket > -1)	// si une nouvelle connexion est arrivée et donc qu'il y a un client à créer
	{
		std::cout << "Accepted new connection on ";
		std::cout << origin.get_host() << ":" << COLOR_BOLD_BLUE << origin.get_port() << COLOR_RESET << ". ";
		std::cout << "new socket on fd: " << COLOR_GREEN << new_socket << COLOR_RESET << "\n";
		_connections.push_back( Connection(origin, new_socket) );
		_add_fd_poll_struct(new_socket, (POLLIN | POLLOUT) );
	}
}

server_iter_type Gateway::decide_server( Connection& connection )
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
			std::cout << "decide_server: Server #" << res->get_id() << " is default\n\n";
		}
	}
	return res;
}

// void	Gateway::close_connections( void )
// {
// 	connection_iter_type connection_iter;
// 	std::vector< pollfd >::iterator poll_struct_iter;

// 	connection_iter = _connections.begin();
// 	poll_struct_iter = poll_struct.begin() + _map_origin_socket.size();
	
// 	while ( connection_iter != _connections.end() )
// 	{
// 		connection_iter->update_close();
// 		if ( connection_iter->get_close() )
// 		{
// 			connection_iter->close_connection(); // potential close routine
// 			// erase connection and corresponding poll_struct
// 			// TODO: remove fd from sockets we listen to ?
// 			connection_iter = _connections.erase(connection_iter);
// 			poll_struct_iter = poll_struct.erase(poll_struct_iter);
// 		}
// 		else
// 		{
// 			++connection_iter;
// 			++poll_struct_iter;
// 		}
// 	}
// }

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
	socket_iter_type it = _map_origin_socket.begin();
	socket_iter_type end = _map_origin_socket.end();

	for ( ; it != end; ++it )
	{
		std::cout << it->first.get_host() << ":" << COLOR_BOLD_BLUE << it->first.get_port() << COLOR_RESET;
		std::cout << " has socket number " << COLOR_GREEN << it->second.fd << COLOR_RESET;
		std::cout << std::endl;
	}
}
