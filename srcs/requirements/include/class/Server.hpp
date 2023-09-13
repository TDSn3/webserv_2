/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/06 17:47:51 by yfoucade          #+#    #+#             */
/*   Updated: 2023/09/13 21:04:22 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <set>
#include <vector>
#include <string>

#include "Connection.hpp"
#include "exceptions.hpp"
#include "Location.hpp"
#include "Origin.hpp"

class Server
{
	public:

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

	private :

		typedef std::vector< std::string >			string_vector;
		typedef std::map< std::string, Location >	location_map;

		static int								_n_servers;
		int										_id;
		bool									_parsing_error;
		std::set< Origin >						_origins;
		std::vector< std::string >				_names;
		std::map< std::string, Location >		_locations;

		bool									is_simple_directive( std::vector< std::string > tokens ) const;
		bool									is_location_directive( std::string& line, std::vector< std::string > tokens ) const;
		void									process_simple_directive(std::string line, std::vector< std::string > tokens );
		void									process_location_directive(std::vector< std::string >::iterator curr ,std::vector< std::string >::iterator last);
		std::vector< std::string >::iterator	find_end_of_location_block(std::vector< std::string >::iterator first, std::vector< std::string >::iterator last);
		void									parse_listen(std::string&, std::vector< std::string >);
		void									parse_server_name(std::vector<std::string>);

};
