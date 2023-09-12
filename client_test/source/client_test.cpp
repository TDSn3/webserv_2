/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_test.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 11:32:22 by tda-silv          #+#    #+#             */
/*   Updated: 2023/09/05 09:43:42 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <header.hpp>

static int	init(const int &argc, char **argv, int &port, std::string &http_request);

/* ************************************************************************** */
/*                                                                            */
/*   argv[1] = PORT                                                           */
/*                                                                            */
/* ************************************************************************** */
int	main(int argc, char **argv)
{
	int					port;
	int					_connexion_fd = 0;
	ssize_t				ret;
	struct sockaddr_in	serv_addr;
	std::string			http_request;
	char				buffer[4096] = {0};

	if (init(argc, argv, port, http_request) )
		return (-1);

	_connexion_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_connexion_fd < 0)
	{
		perror("cannot create socket");
		return (-1);
	}

	memset( (char *) &serv_addr, 0, sizeof(serv_addr) );

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(static_cast<uint16_t>(port) );

	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
	{
		perror("invalid address or address not supported");
		return (-1);
	}

	if (connect(_connexion_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr) ) < 0)
	{
		perror("connection failed");
		return (-1);
	}

	send(_connexion_fd , http_request.c_str() , http_request.size() , 0);
	ret = recv(_connexion_fd , buffer, sizeof(buffer), 0);

	std::cout << COLOR_BOLD_RED << "BUFFER : " << ret << " / " << 4096 << COLOR_RESET << std::endl;
	std::cout << buffer << std::endl;

	return (0);
}

static int	init(const int &argc, char **argv, int &port, std::string &http_request)
{
	if (argc != 2)
	{
		perror("bad number of arguments");
		return (1);
	}

	port = atoi(argv[1]);
 
	if (port <= 0)
	{
		perror("Bad port");
		return (1);
	}

	std::ifstream		file("http_request.txt");
	std::stringstream	buffer;

	if (!file.is_open() )
	{
		perror("failed to open file");
		return (1);
	}

	buffer << file.rdbuf();
	http_request = buffer.str();
	file.close();

	return (0);
}
