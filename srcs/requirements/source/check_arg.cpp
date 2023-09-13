/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_arg.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 10:05:59 by tda-silv          #+#    #+#             */
/*   Updated: 2023/09/13 21:38:51 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

static bool	my_perror( const char *str );

bool	check_arg( const int argc, const char * const *argv )
{
	if ( (argc != 1 && argc != 2) || (argc == 2 && !argv[1]) )
		return ( my_perror( "bad argument" ) );

	struct stat buffer;

	if ( argc == 1 )	// configuration file by default
	{
		if ( stat(DEFAULT_CONF_FILE, &buffer ) != 0 )	// Vérifie si le fichier existe
			return ( my_perror( "default configuration file does not exist" ) );
		if ( access(DEFAULT_CONF_FILE, R_OK) != 0 )	// Vérifie si le fichier est accessible en lecture
			return ( my_perror( "default configuration file is not readable" ) );
	}
	else				// argc == 2
	{
		if ( stat(argv[1], &buffer ) != 0)
			return ( my_perror( "file does not exist" ) );
		if (access(argv[1], R_OK) != 0)
			return ( my_perror( "file is not readable" ) );
	}

	return ( true );
}

static bool	my_perror( const char *str )
{
	std::cerr << COLOR_BOLD_RED << str << COLOR_RESET << std::endl;
	return ( false );
}
