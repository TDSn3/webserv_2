/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _exec_cgi.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 15:19:36 by tda-silv          #+#    #+#             */
/*   Updated: 2023/10/02 20:48:18 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

static void	new_char_for_execve( Request &request, std::vector<char *> &arg_for_execve, std::string &path );
static void	new_char_for_env_update( std::vector<char *> &env_update, char **env, Request &request );
static void	env_update_push_back( std::vector<char *> &env_update, const char *str_to_add );

std::string	HttpResponse::_exec_cgi( std::string path, Request &request, char **env )	// ! throw possible
{
	struct stat			stat_buffer;
	int					pipefd[2];
	int					stdin_pipefd[2];
	pid_t				pid;
	std::string			str;
	std::vector<char *>	arg_for_execve;
	std::vector<char *>	env_update;

	std::cout << COLOR_GREEN << path << COLOR_RESET << "\n";
	if ( stat( path.c_str(), &stat_buffer ) != 0 )		// Vérifie si le fichier existe
		my_perror_and_throw( "cgi file does not exist", StatusCode( 404 ) );
	if ( access( path.c_str(), R_OK | X_OK ) != 0 )		// Vérifie si le fichier est accessible en lecture et en exécution
		my_perror_and_throw( "cgi file is not readable or executable", StatusCode( 403 ) );

	new_char_for_execve( request, arg_for_execve, path );
	new_char_for_env_update( env_update, env, request );

	pipe( pipefd );							// pipefd[0] en lecture, pipefd[1] en écriture
	pipe( stdin_pipefd );

	pid = fork();

	if ( pid == 0 )							// enfant
	{
		close( pipefd[0] ); 				// ferme l'extrémité en lecture
		close( stdin_pipefd[1] );
		dup2( pipefd[1], STDOUT_FILENO );	// redirige stdout vers pipefd[1]
		dup2( stdin_pipefd[0], STDIN_FILENO );
		execve( path.c_str(), arg_for_execve.data(), env_update.data() );
	}
	else if (pid > 0)						// parent
	{
		ssize_t	ret = 1;
		char	buffer[4096];

		close( pipefd[1] );  				// ferme l'extrémité en écriture
		close( stdin_pipefd[0] );
		write( stdin_pipefd[1], request.get_body().c_str(), request.get_body().size() );	// envois le body au CGI
		close( stdin_pipefd[1] );
		while ( ret > 0 )
		{
			memset( buffer, 0, sizeof( buffer ) );
			ret = read( pipefd[0], buffer, sizeof( buffer ) );
			str += buffer;
		}
		waitpid( pid, NULL, 0 );			// TODO: ajouter une limite
	}

	for ( size_t i = 0; i < arg_for_execve.size() -1 ; i++ )
		delete [] arg_for_execve[i];

	for ( size_t i = 0; i < env_update.size() -1 ; i++ )
		delete [] env_update[i];
	
	return ( str );
}

static void	new_char_for_execve( Request &request, std::vector<char *> &arg_for_execve, std::string &path )
{
	char	*str;

	str = new char[ path.size() + 1 ];
	std::strcpy( str, path.c_str() );
	arg_for_execve.push_back( str );

	for ( std::map<std::string, std::string> :: iterator it = request.request_line.parsed_url.query_parameters.begin();
		it != request.request_line.parsed_url.query_parameters.end();
		it++ )
	{
		str = new char[ it->first.size() + 1 + it->second.size() + 1 ];	// + 1 pour le '=' et un autre pour le \0
		std::strcpy( str, (it->first + "=" + it->second).c_str() );
		arg_for_execve.push_back( str );
	}
	arg_for_execve.push_back( NULL );
}

static void	new_char_for_env_update( std::vector<char *> &env_update, char **env, Request &request )
{
	for ( size_t i = 0; env[i]; i++ )
		env_update_push_back( env_update, env[i] );

	env_update_push_back( env_update, ( "REQUEST_METHOD=" + request.request_line.method ).c_str() );
	env_update_push_back( env_update, ( "SERVER_PROTOCOL=" + request.request_line.get_version() ).c_str() );
	env_update_push_back( env_update,  "PATH_INFO=/" );

	env_update.push_back( NULL );
}

static void	env_update_push_back( std::vector<char *> &env_update, const char *str_to_add )
{
	char	*str;
		
	str = new char[ std::string( str_to_add ).size() + 1 ];
	std::strcpy( str, str_to_add );
	env_update.push_back( str );
}
