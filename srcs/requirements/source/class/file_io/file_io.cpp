/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_io.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 17:54:46 by yfoucade          #+#    #+#             */
/*   Updated: 2023/09/12 10:08:31 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

std::vector< std::string > read_file( std::string filename )
{
	std::ifstream				file(filename.c_str() );
	std::string					line;
	std::vector< std::string >	content;

	if ( !file.is_open() )
		throw FileOpeningException(std::string("Cannot open config file: ") + std::string(filename));
	while (std::getline(file, line) )
		content.push_back(line);

	file.close();
	return content;
}
