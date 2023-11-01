/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfoucade <yfoucade@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 12:05:04 by yfoucade          #+#    #+#             */
/*   Updated: 2023/10/31 13:55:20 by yfoucade         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <queue>

#include "../utils.hpp"
#include "Origin.hpp"
#include "Request.hpp"
#include "HttpResponse.hpp"

class Connection
{
	public:

		Connection( void );
		Connection( Origin, int );
		Connection( const Connection& other );
		Connection& operator=( const Connection& other );

		bool					response_status;
		HttpResponse			response;

		Origin&					get_origin( void );
		int						get_socket( void ) const;
		bool					get_close( void ) const;
		Request&				get_request( void );
		void					update_close( void );
		void					set_close( bool value );
		void					close_connection( void );
		void					receive( void );
		bool					is_ready_for_reply( void );
		void					flush_request( void );

	private:

		Origin					_origin;
		int						_socket;
		Request 				_request;
		bool					_close;
		bool					_continue_reading;
		char					_read_buffer[BUFF_SIZE + 1];
		std::string				_parse_buffer;
		std::string::size_type	_line_end;
		// Request& select_request( void );
		// time _last_reception;
		// void set_parse_buffer( std::string new_parse_buffer );

};
