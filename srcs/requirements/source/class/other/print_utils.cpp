/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfoucade <yfoucade@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 14:27:37 by yfoucade          #+#    #+#             */
/*   Updated: 2023/09/14 23:50:21 by yfoucade         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

void	print_string_vector( std::vector< std::string > vec, std::string sep )
{
	std::vector< std::string >::iterator first = vec.begin();
	std::vector< std::string >::iterator last = vec.end();

	for ( ; first != last; ++first )
	{
		std::cout << *first << (first + 1 == last ? "" : sep);
	}
	std::cout << std::endl;
}

