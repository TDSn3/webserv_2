/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_body.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 12:15:44 by tda-silv          #+#    #+#             */
/*   Updated: 2023/09/17 15:15:50 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

std::string	&Request::get_body( void )
{
	return ( _body );
}
