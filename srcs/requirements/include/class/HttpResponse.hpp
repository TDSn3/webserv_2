/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 15:43:46 by tda-silv          #+#    #+#             */
/*   Updated: 2023/09/17 14:08:31 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

# include <iostream>

# include "../utils.hpp"

class Request;

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

		std::string		str_response;

		void			build(Request &request, char **env);						// ! throw possible
		void			build_error(Request &request, const int status_code);
		bool			status(void);
		void			clear(void);

	protected:

	private:

		std::string		_read_file_in_str(std::string path);						// ! throw possible
		std::string		_exec_cgi(std::string path, Request &request, char **env);	// ! throw possible
		void			_make_response(Request &request, char **env);				// ! throw possible
		void			_give_content_type(Request &request);
		void			_make_reason_phrase(void);
		void			_add_status_line(void);
		void			_add_field_line(std::string field_name, std::string field_value);
		void			_add_body(Request &request, char **env);
		void			_add_body(std::string path);
		void			_set_status_line( int code, std::string reason_phrase);
		void			_delete_method( Request &request );							// ! throw possible

};

#endif
