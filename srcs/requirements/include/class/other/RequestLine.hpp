/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestLine.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 22:51:53 by yfoucade          #+#    #+#             */
/*   Updated: 2023/10/02 15:23:53 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

#include "parsing_utils.hpp"

struct s_parsed_url
{
	std::string							scheme;
	std::string							domain;
	std::string							port;
	std::string							path;
	std::map<std::string, std::string>	query_parameters;
	std::string							fragment;
};

class RequestLine
{
	public:

		RequestLine( void );
		RequestLine( const std::string& line );
		RequestLine( const RequestLine& other );
		RequestLine& operator=( const RequestLine& other );
		~RequestLine();

		std::string		method;
		s_parsed_url	parsed_url;

		void			print_request_line( std::string = std::string(), std::string = std::string() );
		void			print_request_line_two( void );
		int				get_major_version( void );
		int				get_minor_version( void );
		std::string		get_version( void );
		bool			has_syntax_error( void );

	private:

		std::string::const_iterator	extract_method( std::string::const_iterator, std::string::const_iterator );
		std::string::const_iterator	extract_target( std::string::const_iterator, std::string::const_iterator );
		std::string::const_iterator	go_to_segment_end( std::string::const_iterator, std::string::const_iterator );
		std::string::const_iterator	extract_version( std::string::const_iterator, std::string::const_iterator );

		std::string					_request_target;
		std::string					_http_version;
		bool						_syntax_is_ok;
		int							_major_version;
		int							_minor_version;

		void						_parse_url();	// ! throw possible
		void						_print_parsing_url();
};
