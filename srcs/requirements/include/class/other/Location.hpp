/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfoucade <yfoucade@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 16:45:48 by yfoucade          #+#    #+#             */
/*   Updated: 2023/10/01 12:31:04 by yfoucade         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <string>
#include <vector>

class Location
{
	public:
		typedef std::map< std::string, std::vector< std::string > > parameters_map;
		std::map< std::string, std::vector< std::string > > _parameters;
		std::string _uri;
		Location();
		Location(
			std::vector< std::string >::iterator first,
			std::vector< std::string >::iterator last );
		Location( const Location& other );
		Location& operator=( const Location& other );
		~Location( void );
		void print_location( void );

	private:
		void	parse_location_line( std::string line, std::vector< std::string > tokens );
		void	set_root( std::string line, std::vector< std::string > tokens );
		void	init_allowed_methods( void );
		void	parse_allowed_methods( std::string, std::vector< std::string > );
		void	parse_return( std::string, std::vector< std::string > );
		void	parse_autoindex( std::string, std::vector< std::string > );
		void	parse_index( std::string, std::vector< std::string > );
		void	parse_cgi( std::string, std::vector< std::string > );
		void	parse_upload_store( std::string, std::vector< std::string > );
};
