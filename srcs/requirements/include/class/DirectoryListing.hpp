/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DirectoryListing.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfoucade <yfoucade@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/21 08:53:03 by yfoucade          #+#    #+#             */
/*   Updated: 2023/10/21 10:07:04 by yfoucade         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.hpp"

typedef struct webserv_dirent_s
{
	std::string name;
	std::string time;
	std::string size;
} webserv_dirent_t;

class DirectoryListing
{
public:
	DirectoryListing( std::string dirname, std::string target_resource );
	~DirectoryListing();
	std::string get_html( void );

private:
	std::string						_dirname;
	std::string						_target_resource;
	size_t							_max_name_length;
	size_t							_max_size_length;
	size_t							_name_time_split;
	size_t							_time_size_split;
	std::vector< webserv_dirent_t > _entries;
	std::vector< std::string >		_html_lines;
	
	void	_set_entries( void );
	void	_set_html_entry_lines( void );
};
