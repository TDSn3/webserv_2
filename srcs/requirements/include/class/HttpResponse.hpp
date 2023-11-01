/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 15:43:46 by tda-silv          #+#    #+#             */
/*   Updated: 2023/11/01 08:19:48 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

# include <iostream>
# include "../utils.hpp"

class Request;
class Server;

struct s_status_line
{
	std::string	version;
	int			code;
	std::string	reason_phrase;
};

/* ************************************************************************** */
/*                                                                            */
/*   status_line = HTTP-Version SP* Status-Code SP* Reason-Phrase CRLF* **    */
/*   status_line = HTTP/1.1 200 OK											  */
/*                                                                            */
/*   SP = Space																  */
/*   CR = Carriage Return (retour chariot)									  */
/*   LF = Line Feed	(saut de ligne)											  */
/*   CRLF = utilisée comme marqueur de fin de ligne pour tous les éléments	  */
/*   du protocole, sauf pour le body										  */
/*                                                                            */
/*   *	RFC 2616 - 2.2 Basic Rules											  */
/*   **	RFC 2616 - 6.1 Status-Line											  */
/*                                                                            */
/* ************************************************************************** */

class HttpResponse
{
	public:

		std::string		data;

		s_status_line	status_line;
		std::string		str_header;
		std::string		str_body;
		size_t			to_send;
		ssize_t			sent;
		size_t			total_sent;

		std::string		str_response;

		void			build(Request &request, char **env, Server& server);						// ! throw possible
		void			build_100_continue(Request &request, Server& server);
		void			build_error(Request &request, const int status_code);
		void			build_error(Request &request, Server& server, const int status_code);
		bool			status(void);
		void			clear(void);

	protected:

	private:

		std::string		_read_file_in_str(std::string path);							// ! throw possible
		std::string		_exec_cgi(std::string &path, Request &request, char **env);		// ! throw possible
		void			_make_response( Request &request, std::string &path );			// ! throw possible
		void			_add_content_type( std::string &path );
		void			_make_reason_phrase(void);
		void			_add_status_line(void);
		void			_add_field_line(std::string field_name, std::string field_value);
		void			_add_body(std::string path);
		void			_set_status_line( int code, std::string reason_phrase);
		bool			_get_method( Request &request, char **env, std::string &path );		// ! throw possible
		bool			_post_method( Request &request, char **env, std::string &path );	// ! throw possible
		void			_delete_method( std::string &path );								// ! throw possible
		void			_build_directory_listing( std::string dirname, std::string target_resource );

/* ************************************************************************** */
/*                                                                            */
/*   build																	  */
/*                                                                            */
/* ************************************************************************** */

		bool			_select_method( Request &request, char **env, Location *location, std::string path );	// ! throw possible
		void			_rewrite_path( std::string &new_path, Location *location, std::string path );
		bool			_autoindex_is_on( Location *location );

};

#endif
