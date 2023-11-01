/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfoucade <yfoucade@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/06 17:47:51 by yfoucade          #+#    #+#             */
/*   Updated: 2023/11/01 22:04:08 by yfoucade         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <set>
#include <vector>
#include <string>

#include "HttpResponse.hpp"
#include "Connection.hpp"
#include "other/exceptions.hpp"
#include "other/Location.hpp"
#include "Origin.hpp"

class Server
{
	public:
		int										_id;
		static int								_n_servers;
		bool									_parsing_error;
		std::set< Origin >						_origins;
		std::vector< std::string >				_names;
		long									_max_client_body_size;
		std::map< int, std::string >			_default_error_pages;
		std::map< std::string, Location >		_locations;
		std::string								root;

		Server( std::vector< std::string >::iterator first, std::vector< std::string >::iterator last);
		Server( const Server& );	// attention pendant la fusion
		~Server();

		void									print_config( void );
		void									set_names( std::vector< std::string > directive_and_names );
		int										get_id( void ) const;
		const std::set< Origin >				&get_origins( void ) const;	// used
		bool									listens_to_origin( const Origin& );
		bool									has_server_name( const std::string& );
		void									reply( Connection &connection, char **env );
		Location*								select_non_cgi_location( std::string path );
		Location*								select_cgi_location( std::string path, std::string method );

	private :

		typedef std::vector< std::string >			string_vector;
		typedef std::map< std::string, Location >	location_map;


		bool									is_simple_directive( std::vector< std::string > tokens ) const;
		bool									is_location_directive( std::string& line, std::vector< std::string > tokens ) const;
		void									process_simple_directive( std::string line, std::vector< std::string > tokens );
		void									process_location_directive( std::vector< std::string >::iterator curr ,std::vector< std::string >::iterator last );
		std::vector< std::string >::iterator	find_end_of_location_block( std::vector< std::string >::iterator first, std::vector< std::string >::iterator last );
		void									parse_listen( std::string&, std::vector< std::string > );
		void									parse_server_name( std::vector<std::string> );
		void									parse_client_max_body_size( std::string line, std::vector<std::string> );
		void									parse_error_page( std::string line, std::vector< std::string > tokens );
		void									parse_root( std::string, std::vector<std::string> );
};
