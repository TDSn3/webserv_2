/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 15:49:59 by tda-silv          #+#    #+#             */
/*   Updated: 2023/09/17 11:30:10 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

# include <vector>

#ifndef DEFAULT_CONF_FILE
# define DEFAULT_CONF_FILE  "conf/webserv.conf"
#endif

#ifndef DEFAULT_HOST
# define DEFAULT_HOST "0.0.0.0"
#endif

#ifndef DEFAULT_PORT
# define DEFAULT_PORT "8080"
#endif

#ifndef BUFF_SIZE
# define BUFF_SIZE 1024
#endif

#ifndef LINE
# define LINE std::string::npos
#endif

#ifndef SUCCESS
# define SUCCESS 0
#endif

#ifndef FAILURE
# define FAILURE 1
#endif

#ifndef HTTP_VERSION
# define HTTP_VERSION "HTTP/1.1"
#endif

#ifndef INDEX_FILE_NAME
# define INDEX_FILE_NAME "card.html"
#endif

#ifndef FAVICON_FILE_NAME
# define FAVICON_FILE_NAME "favicon.png"
#endif

#ifndef ROOT
# define ROOT "www"
#endif

enum e_parsing_status
{
	e_request_line,
	header_section,
	expect_100_continue,
	body,
	chunk_size,
	chunk_data,
	trailer_section,
	done
};

enum e_final_status
{
	parsing,
	success,
	failure,
	bad_request,
	content_length_and_transfer_coding,
	non_chunked_transfer_coding,
	invalid_content_length,
	message_body_but_no_content_length
};

bool						is_whitespace( char c );
std::vector< std::string >	tokenize( std::string str );

extern volatile sig_atomic_t	siginit_status;

#endif
