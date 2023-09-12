/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestLine.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 22:51:53 by yfoucade          #+#    #+#             */
/*   Updated: 2023/09/12 11:30:19 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

#include "parsing_utils.hpp"

class RequestLine
{
	public:

		RequestLine( void );
		RequestLine( const std::string& line );
		RequestLine( const RequestLine& other );
		RequestLine& operator=( const RequestLine& other );
		~RequestLine();

		void	print_request_line( std::string = std::string(), std::string = std::string() );
		int		get_major_version( void );
		int		get_minor_version( void );
		bool	has_syntax_error( void );

	private:

		std::string::const_iterator	extract_method( std::string::const_iterator, std::string::const_iterator );
		std::string::const_iterator	extract_target( std::string::const_iterator, std::string::const_iterator );
		std::string::const_iterator	go_to_segment_end( std::string::const_iterator, std::string::const_iterator );
		std::string::const_iterator	extract_version( std::string::const_iterator, std::string::const_iterator );

		std::string					_method;
		std::string					_request_target;
		std::string					_http_version;
		bool						_syntax_is_ok;
		int							_major_version;
		int							_minor_version;
};
