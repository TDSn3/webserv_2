/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 11:54:40 by yfoucade          #+#    #+#             */
/*   Updated: 2023/11/01 07:33:55 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <string>

#include "../utils.hpp"
#include "other/RequestLine.hpp"

class Request
{
	public:

		Request( void );
		Request( const Request &other );
		Request &operator = ( const Request &other );
		~Request( void );

		typedef std::map< std::string, std::vector< std::string > >	field_section_type;

		RequestLine					request_line;
		field_section_type 			_header_section;

		bool						is_complete( void );
		std::string::size_type		get_required_size( void );
		void						add_data( const std::string & line );
		bool						get_parsing_error( void );
		bool						has_close_option( void );
		e_parsing_status			get_parsing_status( void );
		e_final_status				get_final_status( void );
		void						set_status_after_continue( void );
		std::string					get_host_value( void );
		bool						get_content_length_status( void);
		long						get_content_length_value( void);
		std::string					&get_body( void );
		void						print_request( void );

	private:

		e_parsing_status			_parsing_status;
		e_final_status				_final_status;
		std::string					_content;
		std::string::size_type		_nchars_remaining;
		std::string::size_type		_chunk_size_cumsum;
		std::vector< std::string >	_raw_header_section;
		std::vector< std::string >	_raw_trailer_section;
		field_section_type 			_trailer_section;
		bool						_has_host;
		std::string 				_host_value;
		bool						_has_content_length;
		long						_content_length_value;
		bool						_has_transfer_encoding;
		std::string 				_transfer_encoding_value;
		bool						_has_expect;
		std::string 				_expect_value;
		bool						_has_connection;
		std::string 				_connection_value;
		std::string 				_body;
		time_t						_last_reception;
	
		void						parse_request_line( const std::string& line );
		void						add_header_line( const std::string& line );
		void						parse_body( const std::string& line );
		void						parse_chunk_size( const std::string& line );
		void						parse_chunk_content( const std::string& line );
		void						add_trailer_line( const std::string& line );
		e_parsing_status			get_status_after_header( void );
		std::string					get_field_value( std::string field_name );
		bool						is_valid_content_length( std::string content_length );
		void						parse_field_section( std::vector< std::string >&, field_section_type& );
		bool						set_field_values( field_section_type& );
		void						add_field_to_section( const std::string, field_section_type& );
		void						check_field_syntax( const std::string& line );
		void						print_field_section( field_section_type& );
		bool						set_host_field( field_section_type& );
		bool						set_content_length_field( field_section_type& );
		bool						set_transfer_encoding_field( field_section_type& );
		bool						set_expect_field( field_section_type& );
		bool						set_connection_field( field_section_type& );
};