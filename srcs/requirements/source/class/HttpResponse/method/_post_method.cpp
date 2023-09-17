/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _post_method.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 14:57:55 by tda-silv          #+#    #+#             */
/*   Updated: 2023/09/17 15:23:47 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

/* ************************************************************************** */
/*                                                                            */
/*   std::ios::out		Ouvre le fichier en mode écriture. Si le fichier	  */
/*   n'existe pas, il sera créé. Si le fichier existe déjà,					  */
/*   son contenu sera écrasé.												  */
/*                                                                            */
/*   std::ios::binary	Ouvre le fichier en mode binaire,					  */
/*                                                                            */
/* ************************************************************************** */
void	HttpResponse::_post_method( Request &request )	// ! throw possible
{
	std::string		path;
	std::string		new_path;
	std::ofstream	out_file;

	path = request.request_line.parsed_url.path;
	if ( !path.empty() && path[0] == '/' )
		new_path = std::string( ROOT ) + path;
	else if ( !path.empty() && path[0] != '/' )
		new_path = std::string( ROOT ) + "/" + path;

	out_file.open( new_path.c_str() , std::ios::out | std::ios::binary );

	if ( out_file.fail() )
		my_perror_and_throw( "POST: internal server error", StatusCode(500) );

	out_file.write( request.get_body().c_str(), request.get_body().size() );

	if ( out_file.good() == false )
		my_perror_and_throw( "POST: internal server error", StatusCode(500) );

	out_file.close();
	_set_status_line( 201, "Created" );
}
