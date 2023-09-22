/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_perror.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:03:57 by tda-silv          #+#    #+#             */
/*   Updated: 2023/09/22 16:05:59 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

bool	my_perror( const char *str )
{
	std::cerr << COLOR_BOLD_RED << str << COLOR_RESET << std::endl;
	return ( false );
}
