/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_perror_and_throw.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 11:46:11 by tda-silv          #+#    #+#             */
/*   Updated: 2023/09/13 15:50:02 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

void	my_perror_and_throw(const char *str, const std::exception &e)
{
	std::cerr << COLOR_BOLD_RED << str << COLOR_RESET << std::endl;
	throw (e);
}

void	my_perror_and_throw(const char *str, const StatusCode &e)
{
	std::cerr << COLOR_BOLD_RED << str << COLOR_RESET << std::endl;
	throw (e);
}
