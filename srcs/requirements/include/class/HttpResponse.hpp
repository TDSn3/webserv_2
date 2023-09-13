/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 15:43:46 by tda-silv          #+#    #+#             */
/*   Updated: 2023/09/13 11:45:07 by tda-silv         ###   ########.fr       */
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

		void	build(Request &request, char **env)		// ! throw possible
		{
			status_line.version = std::string(HTTP_VERSION) ;
			status_line.code = 200;
			status_line.reason_phrase = "OK";

			_make_response(request, env);				// ! throw possible
		};

		void	build_error(Request &request, const int status_code)
		{
			std::ostringstream	oss;
			
			(void) request;
			status_line.version = std::string(HTTP_VERSION);
			status_line.code = status_code;
			_make_reason_phrase();

			_add_status_line();

			_add_field_line("server", "webserv");
			_add_field_line("content-type", "text/html");
			
			oss << status_code;
			_add_body("error_page/" + oss.str() + ".html");
		};

		bool	status(void)
		{
			if (str_response.empty() )
				return (false);	// vide
			return (true);		// remplie
		};

		void	clear(void)
		{
			data.clear();
			str_header.clear();
			str_body.clear();

			status_line.version.clear();
			status_line.code = 0;
			status_line.reason_phrase.clear();

			str_response.clear();
		};

		std::string		data;

		s_status_line	status_line;
		std::string		str_header;
		std::string		str_body;

		std::string		str_response;

	protected:

	private:

		std::string		_read_file_in_str(std::string path);							// ! throw possible
		std::string		_exec_cgi(std::string path, Request &request, char **env);	// ! throw possible
		void			_make_response(Request &request, char **env);				// ! throw possible
		void			_give_content_type(Request &request);
		void			_make_reason_phrase(void);
		void			_add_status_line(void);
		void			_add_field_line(std::string field_name, std::string field_value);
		void			_add_body(Request &request, char **env);
		void			_add_body(std::string path);

};

#endif
