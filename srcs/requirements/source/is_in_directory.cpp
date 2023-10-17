/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_in_directory.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 10:54:27 by tda-silv          #+#    #+#             */
/*   Updated: 2023/10/02 10:54:49 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

bool	is_in_directory( std::string dir, std::string file )
{
	DIR				*directory = opendir(dir.c_str());
	struct dirent	*dir_entry;
	bool			ret = false;

	if ( !directory )
		return false;
	while ( (dir_entry = readdir(directory)) )
	{
		if ( file == dir_entry->d_name )
		{
			ret = true;
			break;
		}
	}
	closedir(directory);
	return ret;
}
