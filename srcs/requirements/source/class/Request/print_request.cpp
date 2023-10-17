/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_request.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 15:51:43 by tda-silv          #+#    #+#             */
/*   Updated: 2023/10/07 20:31:50 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

void	Request::print_request( void )
{
	std::cout << COLOR_BOLD_BLUE;

	request_line.print_request_line_two();

	std::cout << COLOR_RESET << COLOR_BLUE;

	for ( std::vector< std::string > :: iterator it = _raw_header_section.begin(); it != _raw_header_section.end(); it++ )
		std::cout << *it << "\n";

	std::cout << "\n" << _body.substr(0, 300);

	for ( std::vector< std::string > :: iterator it = _raw_trailer_section.begin(); it != _raw_trailer_section.end(); it++ )
		std::cout << *it << "\n";

	std::cout << COLOR_RESET << std::endl;
}
