/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clear.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 11:02:40 by tda-silv          #+#    #+#             */
/*   Updated: 2023/10/04 15:47:03 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

void	HttpResponse::clear(void)
{
	data.clear();
	str_header.clear();
	str_body.clear();
	to_send = 0;
	sent = 0;
	total_sent = 0;

	status_line.version.clear();
	status_line.code = 0;
	status_line.reason_phrase.clear();

	str_response.clear();
};
