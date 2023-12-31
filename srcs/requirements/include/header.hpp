/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfoucade <yfoucade@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 15:58:36 by tda-silv          #+#    #+#             */
/*   Updated: 2023/11/02 12:16:11 by yfoucade         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_HPP
# define HEADER_HPP

# include <unistd.h>
# include <string.h>
# include <stdio.h>
# include <errno.h>
# include <arpa/inet.h>
# include <sys/select.h>
# include <sys/time.h>
# include <sys/types.h>
# include <poll.h>
# include <sys/socket.h>
# include <netdb.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <signal.h>
# include <sys/stat.h>
# include <dirent.h>
# include <netinet/in.h>
# include <netdb.h>

# include <string>
# include <cstring>
# include <iostream>
# include <sstream>
# include <fstream>
# include <vector>
# include <queue>
# include <map>
# include <exception>
# include <algorithm>
# include <climits>
# include <cstdlib>
# include <cstdio>

# include "colors.hpp"
# include "utils.hpp"

# include "class/other/exceptions.hpp"
# include "class/other/file_io.hpp"
# include "class/other/Location.hpp"
# include "class/other/LogFile.hpp"
# include "class/other/print_utils.hpp"
# include "class/other/parsing_utils.hpp"
# include "class/other/RequestLine.hpp"
# include "class/HttpResponse.hpp"
# include "class/other/StatusCode.hpp"

# include "class/CGIResponse.hpp"
# include "class/Connection.hpp"
# include "class/DirectoryListing.hpp"
# include "class/Gateway.hpp"
# include "class/Origin.hpp"
# include "class/Request.hpp"
# include "class/Server.hpp"

bool	check_arg( const int argc, const char * const *argv );
void	handler( int sig );
bool	my_perror( const char *str );
void	my_perror_and_throw( const char *str, const std::exception &e );
void	my_perror_and_throw( const char *str, const StatusCode &e );
bool	is_in_directory( std::string dir, std::string file );
bool	is_directory( std::string name );
void	listen_loop( Gateway &gateway, char **env );	// ! throw possible
bool	is_allowed_methods( Location *location, std::string method );

#endif
