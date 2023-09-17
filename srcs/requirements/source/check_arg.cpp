/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_arg.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 10:05:59 by tda-silv          #+#    #+#             */
/*   Updated: 2023/09/17 10:16:34 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

static bool	my_perror( const char *str );

bool	check_arg( const int argc, const char * const *argv )
{
	if ( ( argc != 1 && argc != 2 ) || ( argc == 2 && !argv[1] ) )
		return ( my_perror( "bad argument" ) );

	std::string	conf_file;
	struct stat	buffer;

	if ( argc == 1 )	// configuration file by default
		conf_file = DEFAULT_CONF_FILE;

	else				// argc == 2
		conf_file = argv[1];

	if ( stat(conf_file.c_str(), &buffer ) != 0)
		return ( my_perror( "file does not exist" ) );
	if (access(conf_file.c_str(), R_OK) != 0)
		return ( my_perror( "file is not readable" ) );

	return ( true );
}

static bool	my_perror( const char *str )
{
	std::cerr << COLOR_BOLD_RED << str << COLOR_RESET << std::endl;
	return ( false );
}
