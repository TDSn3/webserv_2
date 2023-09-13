/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 15:58:36 by tda-silv          #+#    #+#             */
/*   Updated: 2023/09/13 15:31:11 by tda-silv         ###   ########.fr       */
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

# include "colors.hpp"
# include "utils.hpp"

# include "class/Connection.hpp"
# include "class/exceptions.hpp"
# include "class/file_io.hpp"
# include "class/Gateway.hpp"
# include "class/HttpResponse.hpp"
# include "class/Location.hpp"
# include "class/LogFile.hpp"
# include "class/Origin.hpp"
# include "class/print_utils.hpp"
# include "class/parsing_utils.hpp"
# include "class/Request.hpp"
# include "class/RequestLine.hpp"
# include "class/Server.hpp"

void	handler(int sig);

#endif
