/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_100_continue.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfoucade <yfoucade@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 09:36:05 by yfoucade          #+#    #+#             */
/*   Updated: 2023/10/19 15:34:41 by yfoucade         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

void	HttpResponse::build_100_continue( Request &request, Server& server )
{
	/*
	If the request's parsing_status is `expect_100_continue', it means that
	a body is expected (otherwise, status is done, and we send a final response).
	If a parsing error has occured (bad_request) then we wouldn't end up here neither.
	Thus, the only errors that can happen from here can be checked for after having
	received the body. This means that we can reply with status code 100, and check
	for errors later on.
	*/

	(void)request;
	(void)server;

	std::cout << "Building 100 continue\n";
	str_response = "HTTP/1.1 100 Continue\r\n";
	to_send =  str_response.size();
	sent = 0;
	total_sent = 0;
}
