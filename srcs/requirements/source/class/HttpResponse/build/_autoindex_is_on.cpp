/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _autoindex_is_on.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 10:56:58 by tda-silv          #+#    #+#             */
/*   Updated: 2023/10/02 10:57:44 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

bool	HttpResponse::_autoindex_is_on( Location *location )
{
	if ( !location
	|| location->_parameters.find("autoindex") == location->_parameters.end()
	|| location->_parameters["autoindex"][0] != "on" )
		return false;
	return true;
}
