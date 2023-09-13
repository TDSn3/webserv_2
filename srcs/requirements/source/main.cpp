/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 15:58:01 by tda-silv          #+#    #+#             */
/*   Updated: 2023/09/13 15:31:33 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

volatile sig_atomic_t	siginit_status = 0;

int	main(int argc, char **argv, char **env)
{
	// LogFile		log_file;	// redirige cout et cerr vers le fichier log

	if ( argc > 2 )
	{
		std::cerr << "Usage: webserv [config_file]" << std::endl;
		return(1);
	}

	signal(SIGINT, handler);

	try
	{
		Gateway gateway( argc == 1 ? DEFAULT_CONF_FILE : argv[1] );

		gateway.listen_loop(env);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}

	return (0);
}
