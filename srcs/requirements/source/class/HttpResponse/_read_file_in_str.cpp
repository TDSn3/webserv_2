/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _read_file_in_str.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfoucade <yfoucade@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 11:12:26 by tda-silv          #+#    #+#             */
/*   Updated: 2023/10/01 17:54:57 by yfoucade         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

std::string	HttpResponse::_read_file_in_str( std::string path )	// ! throw possible
{
	std::ifstream		file;
	std::ostringstream	oss;
	std::string			str;

	file.open( path.c_str() );
	if ( !file.good() || !file.is_open() )
	{
		file.close();
		my_perror_and_throw( "Error: could not open file", StatusCode( 404 ) );
	}

    oss << file.rdbuf();
	str = oss.str();

	file.close();

	return (str);
}
