/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _read_file_in_str.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 11:12:26 by tda-silv          #+#    #+#             */
/*   Updated: 2023/09/13 11:52:42 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

std::string	HttpResponse::_read_file_in_str(std::string path)	// ! throw possible
{
	std::string			new_path;
	std::ifstream		file;
	std::ostringstream	oss;
	std::string			str;

	if (!path.empty() && path[0] == '/')
		new_path = std::string(ROOT) + path;
	else if (!path.empty() && path[0] != '/')
		new_path = std::string(ROOT) + "/" + path;

	file.open(new_path.c_str() );
	if (!file.good() || !file.is_open() )
	{
		file.close();
		// my_perror_and_throw("Error: could not open file.", StatusCode(404) );
	}

    oss << file.rdbuf();
	str = oss.str();

	file.close();

	return (str);
}
