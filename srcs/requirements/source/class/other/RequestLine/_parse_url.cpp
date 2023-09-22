/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _parse_url.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 10:01:53 by tda-silv          #+#    #+#             */
/*   Updated: 2023/09/22 16:30:12 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

/* ************************************************************************** */
/*                                                                            */
/*   schéma://hôte:port/chemin?query#fragment								  */
/*   																		  */
/*   schéma		: indique le protocole utilisé (http, https, ftp, etc.)		  */
/*   hôte		: nom de domaine ou adresse IP du serveur					  */
/*   port		: port du serveur (facultatif)								  */
/*   chemin		: chemin d'accès à la ressource sur le serveur				  */
/*   query		: paramètres supplémentaires (facultatif)					  */
/*   fragment	: ancre à une section spécifique de la ressource (facultatif) */
/*                                                                            */
/* ************************************************************************** */
void	RequestLine::_parse_url()	// ! throw possible
{	
	s_parsed_url	&parsed = parsed_url;
	size_t			pos;
	size_t			pos2;
	std::string		stock_str;

/* ************************************************************************** */
/*                                                                            */
/*   schéma																	  */
/*                                                                            */
/* ************************************************************************** */

	pos = _request_target.find("://");
	if (pos != std::string::npos)
	{
		parsed.scheme = _request_target.substr(0, pos);	// ! throw possible substr()
		stock_str = _request_target.substr(pos + 3);	// ! throw possible substr()
	}
	else
		stock_str = _request_target;

/* ************************************************************************** */
/*                                                                            */
/*   nom de domaine															  */
/*                                                                            */
/* ************************************************************************** */

	pos = stock_str.find(":");
	pos2 = stock_str.find("/");
	if (pos != std::string::npos && pos < pos2)			// si ':' est trouvé et qu'il est situé avant '/'
	{
		parsed.domain = stock_str.substr(0, pos);
		stock_str = stock_str.substr(pos + 1);
	}
	else if (pos2 != std::string::npos)
	{
		parsed.domain = stock_str.substr(0, pos2);		// ! throw possible substr()
		stock_str = stock_str.substr(pos2);				// ! throw possible substr()
	}
	else												// aucun '/'
	{
		parsed.domain = stock_str;
		return ;
	}

/* ************************************************************************** */
/*                                                                            */
/*   port																	  */
/*                                                                            */
/* ************************************************************************** */

	pos = stock_str.find("/");
	if (pos != std::string::npos)
	{
		parsed.port = stock_str.substr(0, pos);			// ! throw possible substr()
		stock_str = stock_str.substr(pos);				// ! throw possible substr()
	}
	else												// aucun '/'
	{
		parsed.port = stock_str;
		return ;
	}

/* ************************************************************************** */
/*                                                                            */
/*   path																	  */
/*                                                                            */
/* ************************************************************************** */

	pos = stock_str.find("?");
	pos2 = stock_str.find("#");
	if (pos != std::string::npos)
	{
		parsed.path = stock_str.substr(0, pos);			// ! throw possible substr()
		stock_str = stock_str.substr(pos + 1);			// ! throw possible substr()
	}
	else if (pos2 != std::string::npos)
	{
		parsed.path = stock_str.substr(0, pos2);		// ! throw possible substr()
		stock_str = stock_str.substr(pos2 + 1);			// ! throw possible substr()
	}
	else
	{
		parsed.path = stock_str;
		return ;
	}

/* ************************************************************************** */
/*                                                                            */
/*   query																	  */
/*                                                                            */
/* ************************************************************************** */

	pos2 = stock_str.find("#");
	if (pos != std::string::npos)						// si '?' est trouvé
	{
		std::string			query;

		if (pos2 != std::string::npos)
			query = stock_str.substr(0, pos2);			// ! throw possible substr()
		else
			query = stock_str;

		std::stringstream	ss(query);
		std::string			stock_line;
		size_t				equal;
		std::string			key;
		std::string			value;

		while (std::getline(ss, stock_line, '&') )
		{
			equal = stock_line.find("=");
			if (equal != std::string::npos)
			{
				key = stock_line.substr(0, equal);		// ! throw possible substr()
				value = stock_line.substr(equal + 1);	// ! throw possible substr()
				parsed.query_parameters[key] = value;
			}
		}
	}

/* ************************************************************************** */
/*                                                                            */
/*   fragment																  */
/*                                                                            */
/* ************************************************************************** */

	if (pos2 != std::string::npos)
		parsed.fragment = stock_str.substr(pos2 + 1);
}

void	RequestLine::_print_parsing_url()
{
	s_parsed_url	&parsed = parsed_url;

	std::cout << COLOR_DIM_BLUE;
	std::cout << "Scheme   : " << parsed.scheme << std::endl;
	std::cout << "Domain   : " << parsed.domain << std::endl;
	std::cout << "Port     : " << parsed.port << std::endl;
	std::cout << "Path     : " << parsed.path << std::endl;
	std::cout << "Query    :" << std::endl;
	for (std::map<std::string, std::string>::iterator it = parsed.query_parameters.begin(); it != parsed.query_parameters.end(); ++it)
	{
		std::cout << "\t" << it->first << ": " << it->second << std::endl;
	}
	std::cout << "Fragment : " << parsed.fragment;
	std::cout << COLOR_RESET << "\n\n";
}
