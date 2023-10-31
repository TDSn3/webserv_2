/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_port_part.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfoucade <yfoucade@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 22:13:49 by yfoucade          #+#    #+#             */
/*   Updated: 2023/10/31 22:15:32 by yfoucade         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.hpp"

std::string	remove_port_part( const std::string &host_field_value )
{
	if ( host_field_value.find(':') == std::string::npos )
		return host_field_value;
	return host_field_value.substr(0, host_field_value.find(':'));
}