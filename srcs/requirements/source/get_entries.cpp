/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_entries.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfoucade <yfoucade@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 23:18:39 by yfoucade          #+#    #+#             */
/*   Updated: 2023/10/20 23:26:52 by yfoucade         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.hpp"

std::vector< struct dirent > get_entries( std::string dirname )
{
	DIR								*directory = opendir(dirname.c_str()); // TODO: handle error
	struct dirent					*dir_entry;
	std::vector< struct dirent >	res;

	while ( (dir_entry = readdir(directory)) )
		res.push_back(*dir_entry);
	closedir(directory);
	return res;
}