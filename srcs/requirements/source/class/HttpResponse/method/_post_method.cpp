/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _post_method.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfoucade <yfoucade@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 14:57:55 by tda-silv          #+#    #+#             */
/*   Updated: 2023/11/01 01:53:41 by yfoucade         ###   ########.fr       */
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

bool	HttpResponse::_post_method( Request &request, char **env, std::string &path, Location &location, Server &server )	// ! throw possible
{
	std::ofstream	out_file;
	std::string		out_path;

	if ( path.find( "cgi-bin/" ) != std::string::npos )
	{
		std::cout << COLOR_BOLD_CYAN << "CGI detected" << COLOR_RESET << std::endl;
		str_response = _exec_cgi( path, request, env );				// ! throw possible
		return ( true );	
	}

	out_path = _resolve_out_path( location, path, server );
	out_file.open( out_path.c_str() , std::ios::out | std::ios::binary );

	if ( out_file.fail() )
		my_perror_and_throw( "HttpResponse::_post_method: internal server error", StatusCode( 500 ) );

	out_file.write( request.get_body().c_str(), request.get_body().size() );

	if ( out_file.good() == false )
		my_perror_and_throw( "HttpResponse::_post_method: internal server error", StatusCode( 500 ) );

	out_file.close();
	_set_status_line( 201, "Created" );
	str_header += "Location: " + out_path.substr( server.root.size() ) + "\r\n";
	std::cout << "str_response: " << str_response << std::endl;
	return ( false );
}

std::string		HttpResponse::_resolve_out_path( Location &location, std::string &path, Server &server )
{
	std::string	ret;
	char		timebuf[256];
	
	if ( location._parameters.find("upload_store") == location._parameters.end() )
		return path;

	time_t rawtime;
	// struct tm *ptm;
	time ( &rawtime );
	// ptm = gmtime ( &rawtime );
	strftime( timebuf, 256, "%d-%b-%Y_%H:%M:%S", gmtime( &rawtime ) );

	ret = server.root + "/" + location._parameters["upload_store"][0] + "/" + timebuf;
	return ret;
}