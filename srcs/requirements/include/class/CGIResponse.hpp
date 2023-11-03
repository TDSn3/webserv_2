/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIResponse.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfoucade <yfoucade@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 12:15:28 by yfoucade          #+#    #+#             */
/*   Updated: 2023/11/02 13:00:12 by yfoucade         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "header.hpp"

class CGIResponse {
	public:
		std::string					status_line;
		std::vector< std::string >	headers;
		std::string					body;
		std::string					raw_data;
		std::string					parsed_response;

		CGIResponse( std::string raw_cgi_response );
		~CGIResponse();
	
	private:
		size_t index;
		
		std::string	get_next_line( void );
};