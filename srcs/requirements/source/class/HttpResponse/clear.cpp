/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clear.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 11:02:40 by tda-silv          #+#    #+#             */
/*   Updated: 2023/09/17 11:03:00 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

void	HttpResponse::clear(void)
{
	data.clear();
	str_header.clear();
	str_body.clear();

	status_line.version.clear();
	status_line.code = 0;
	status_line.reason_phrase.clear();

	str_response.clear();
};
