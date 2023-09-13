/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _add_field_line.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 19:31:01 by tda-silv          #+#    #+#             */
/*   Updated: 2023/09/05 19:33:51 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

void	HttpResponse::_add_field_line(std::string field_name, std::string field_value)
{
	str_response += field_name;
	str_response += ": ";
	str_response += field_value;
	str_response += "\r\n";
}
