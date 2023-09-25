/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _set_status_line.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 12:31:50 by tda-silv          #+#    #+#             */
/*   Updated: 2023/09/17 12:32:34 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

void	HttpResponse::_set_status_line( int code, std::string reason_phrase)
{
	status_line.version = std::string(HTTP_VERSION);
	status_line.code = code;
	status_line.reason_phrase = reason_phrase;
}
