/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_directory.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 10:55:28 by tda-silv          #+#    #+#             */
/*   Updated: 2023/10/02 10:56:01 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

bool	is_directory( std::string name )
{
	DIR	*directory = opendir(name.c_str());
	if ( !directory )
		return false;
	closedir(directory);
	return true;
}
