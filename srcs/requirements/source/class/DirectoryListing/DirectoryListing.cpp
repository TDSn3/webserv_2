/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DirectoryListing.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/21 09:07:41 by yfoucade          #+#    #+#             */
/*   Updated: 2023/11/01 12:38:59 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.hpp"

DirectoryListing::DirectoryListing( std::string dirname, std::string target_resource ):
_dirname(dirname),
_target_resource(target_resource),
_max_name_length(0),
_max_size_length(0),
_name_time_split(DIR_LISTING_NAME_TIME_SPLIT),
_time_size_split(DIR_LISTING_TIME_SIZE_SPLIT)
{
	_set_entries();
	_set_html_entry_lines();
}

void	DirectoryListing::_set_entries( void )
{
	DIR				*directory = opendir(_dirname.c_str()); // TODO: handle error
	struct dirent	*dir_entry;
	std::string		name;
	std::string		time;
	std::string		size;
	struct stat		statbuf;
	int				stat_ret;
	char			timebuf[256];
	std::ostringstream	convert;
	webserv_dirent_t	tmp;

	while ( (dir_entry = readdir(directory)) )
	{
		if ( std::string(dir_entry->d_name) == "." || std::string(dir_entry->d_name) == ".." )
			continue;
		name = std::string(dir_entry->d_name);
		stat_ret = stat( (_dirname + "/" + name).c_str(), &statbuf ); // TODO: handle error
		if ( stat_ret == -1 )
			continue;
		if ( is_directory( _dirname + "/" + name ))
		{
			name += "/";
			size = "-";
		}
		else
		{
			convert.str("");
			convert << statbuf.st_size;
			size = convert.str();
		}
		strftime( timebuf, 256, "%d-%b-%Y %H:%M", gmtime( &statbuf.st_mtimespec.tv_sec ) ); // Mac: st_mtimespec ; Linux: st_mtim
		
		_max_name_length = std::max( _max_name_length, name.size() );
		_max_size_length = std::max( _max_size_length, size.size() );
		// std::cout << "_set_entries: " << name << " " << timebuf << " " << size << std::endl;
		tmp.name = name;
		tmp.time = timebuf;
		tmp.size = size;
		_entries.push_back( tmp );
	}
	closedir(directory);
}

void	DirectoryListing::_set_html_entry_lines( void )
{
	std::vector< webserv_dirent_t >::iterator	it = _entries.begin();
	std::vector< webserv_dirent_t >::iterator	end = _entries.end();
	std::string									curr_line;
	size_t										size;

	for ( ; it != end; ++it )
	{
		// curr_line = "<a href=\"" + _target_resource.substr(1) + "/" + it->name + "\">";
		curr_line = "<a href=\"" + it->name + "\">";
		curr_line += it->name;
		curr_line += "</a>";
		
		size = _name_time_split + _max_name_length - it->name.size();
		curr_line += std::string( size, ' ' );
		curr_line += it->time;
		size = _time_size_split + _max_size_length - it->size.size();
		curr_line += std::string( size, ' ' );
		curr_line += it->size;

		_html_lines.push_back(curr_line);
	}
}

std::string	DirectoryListing::get_html( void )
{
	std::string								res;
	std::vector< std::string >::iterator	it = _html_lines.begin();
	std::vector< std::string >::iterator	end = _html_lines.end();

	res = "<html>";
	res += "<head><title>Index of " + _target_resource + "</title></head>";
	res += "<body><h1>Index of " + _target_resource + "</h1><hr><pre>";
	res += "<a href=\"../\">../</a>\n";
	// Add one <a> per file here
	for ( ; it != end; ++it )
		res += *it + "\n";
	res += "</pre><hr></body></html>";
	return res;
}

DirectoryListing::~DirectoryListing()
{}
