/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _exec_cgi.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 15:19:36 by tda-silv          #+#    #+#             */
/*   Updated: 2023/10/05 10:00:16 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

static void	new_char_for_execve( Request &request, std::vector<char *> &arg_for_execve, std::string &path );
static void	new_char_for_env_update( std::vector<char *> &env_update, char **env, Request &request );
static void	env_update_push_back( std::vector<char *> &env_update, const char *str_to_add );
static void	write_all_body( std::string &body, int stdin_pipefd[2] );
static void fork_child( std::string &path, std::vector<char *> &arg_for_execve, std::vector<char *> &env_update, int stdout_pipefd[2], int stdin_pipefd[2] );
static void fork_parent( Request &request, int pid, std::string &str,  int stdout_pipefd[2], int stdin_pipefd[2] );

std::string	HttpResponse::_exec_cgi( std::string &path, Request &request, char **env )	// ! throw possible
{
	struct stat			stat_buffer;
	int					stdout_pipefd[2];
	int					stdin_pipefd[2];
	pid_t				pid;
	std::string			str;
	std::vector<char *>	arg_for_execve;
	std::vector<char *>	env_update;

	if ( stat( path.c_str(), &stat_buffer ) != 0 )		// Vérifie si le fichier existe
		my_perror_and_throw( "cgi file does not exist", StatusCode( 404 ) );
	if ( access( path.c_str(), R_OK | X_OK ) != 0 )		// Vérifie si le fichier est accessible en lecture et en exécution
		my_perror_and_throw( "cgi file is not readable or executable", StatusCode( 403 ) );

	new_char_for_execve( request, arg_for_execve, path );
	new_char_for_env_update( env_update, env, request );

	pipe( stdout_pipefd );								// stdout_pipefd[0] en lecture, stdout_pipefd[1] en écriture
	pipe( stdin_pipefd );

	pid = fork();

	if ( pid == 0 )
		fork_child( path, arg_for_execve, env_update, stdout_pipefd, stdin_pipefd );
	else if (pid > 0)
		fork_parent( request, pid, str, stdout_pipefd, stdin_pipefd );

	for ( size_t i = 0; i < arg_for_execve.size() -1 ; i++ )
		delete [] arg_for_execve[i];

	for ( size_t i = 0; i < env_update.size() -1 ; i++ )
		delete [] env_update[i];







	// std::size_t pos = str.find("\n");
	// if (pos != std::string::npos) { // Si \n a été trouvé
	//     str.erase(0, pos + 1); // Supprimer la première ligne
	// }






// size_t i = 0;
// size_t size = 0;
// 	while (str[i])
// 	{
// 		size++;

// 		if (str[i] == '\n')
// 			size = 0;

// 		i++;
// 	}

// 	std::ostringstream oss;
// 	oss << size;

	// str = "HTTP/1.1 200 OK\nContent-Length: " + oss.str() + "\n" + str;







	// pos = str.find("\n");
	// if (pos != std::string::npos) { // Si \n a été trouvé
	//     str.erase(0, pos + 1); // Supprimer la première ligne
	// }
	// pos = str.find("\n");
	// if (pos != std::string::npos) { // Si \n a été trouvé
	//     str.erase(0, pos + 1); // Supprimer la première ligne
	// }

	// std::ostringstream oss;
	// oss << str.size();

	std::string sstr = std::string("HTTP/1.1 200 OK") + std::string("\nContent-Type: text/html; charset=utf-8\nContent-Length: 100000000\n\n");
	for (size_t i = 0; i < 100000000 ; i++)
		sstr += str.back();
	// sstr = sstr + str;

	return ( sstr );
}

static void fork_child( std::string &path, std::vector<char *> &arg_for_execve, std::vector<char *> &env_update, int stdout_pipefd[2], int stdin_pipefd[2] )
{
	close( stdout_pipefd[0] ); 					// ferme l'extrémité en lecture
	close( stdin_pipefd[1] );

	dup2( stdout_pipefd[1], STDOUT_FILENO );	// redirige stdout vers stdout_pipefd[1]
	dup2( stdin_pipefd[0], STDIN_FILENO );

	execve( path.c_str(), arg_for_execve.data(), env_update.data() );

	exit( 1 );
}

static void fork_parent( Request &request, int pid, std::string &str,  int stdout_pipefd[2], int stdin_pipefd[2] )
{
	ssize_t	ret;
	char	buffer[4096];

	close( stdout_pipefd[1] );  				// ferme l'extrémité en écriture
	close( stdin_pipefd[0] );

	write_all_body( request.get_body(), stdin_pipefd );	// envois le body au CGI
	close( stdin_pipefd[1] );

	ret = 1;
	while ( ret > 0 )
	{
		memset( buffer, 0, sizeof( buffer ) );
		ret = read( stdout_pipefd[0], buffer, sizeof( buffer ) );
		std::cout << COLOR_MAGENTA << ret << COLOR_RESET << "\n";
		str += buffer;
	}
	std::cout << COLOR_MAGENTA << "size : " << str.size() << COLOR_RESET << "\n\n";
	std::cout << COLOR_MAGENTA << str << COLOR_RESET << "\n";

	waitpid( pid, NULL, 0 );					// TODO: ajouter une limite
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

	env_update_push_back( env_update, "SERVER_SOFTWARE=Webserv/1.0" );
	// env_update_push_back( env_update, "SERVER_NAME=localhost" );
	env_update_push_back( env_update, "GATEWAY_INTERFACE=CGI/1.1" );
	env_update_push_back( env_update, "SERVER_PROTOCOL=HTTP/1.1" );
	env_update_push_back( env_update, "SERVER_PORT=8080" );
	// env_update_push_back( env_update, "SCRIPT_NAME=/directory/youpi.bla" );
	// env_update_push_back( env_update, "SCRIPT_FILENAME=/Users/thomas/Desktop/42/webserv_2/srcs/requirements/cgi_tester" );
	env_update_push_back( env_update, "REDIRECT_STATUS=200" );


	env_update_push_back( env_update, ( "REQUEST_METHOD=" + request.request_line.method ).c_str() );
	
	env_update_push_back( env_update,  "PATH_INFO=/" );
	// env_update_push_back( env_update,  "PATH_INFO=/Users/thomas/Desktop/42/webserv_2/srcs/requirements/cgi_tester" );
	
	env_update_push_back( env_update,  "CONTENT_TYPE=application/x-www-form-urlencoded" );
	
	env_update_push_back( env_update,  "QUERY_STRING=" );

	env_update_push_back( env_update, "CONTENT_LENGTH=100000000" );

	env_update.push_back( NULL );
}

static void	env_update_push_back( std::vector<char *> &env_update, const char *str_to_add )
{
	char	*str;
		
	str = new char[ std::string( str_to_add ).size() + 1 ];
	std::strcpy( str, str_to_add );
	env_update.push_back( str );
}

static void	write_all_body( std::string &body, int stdin_pipefd[2] )
{
	// ssize_t written;
	// ssize_t	total_written;
	// ssize_t size_body;

	// total_written = 0;
	// size_body = body.size();
	// int i = 0;
	// while ( total_written < size_body )
	// {
	// 	std::cout << COLOR_RED << i++ << COLOR_RESET<< "\n";
	// 	written = write( stdin_pipefd[1], body.c_str(), 65536 );	// 64 ko = 65536
	// 	std::cout << COLOR_RED << "written : " << written << COLOR_RESET<< "\n";
	// 	std::cout << COLOR_RED << "body size : " << body.size() << COLOR_RESET<< "\n";
	// 	total_written += written;
	// 	body.erase( 0, written );
	// }
	// std::cout << COLOR_RED << "end" << COLOR_RESET<< "\n";

	// write( stdin_pipefd[1], body.c_str(), 100 );	// 64 ko = 65536

	size_t	to_write;
	ssize_t	written;
	size_t	total_written;

	to_write = body.size();
	written = 0;
	total_written = 0;

	std::cout << COLOR_BOLD_YELLOW << "body size : " << body.size() << std::endl;
	std::cout << COLOR_BOLD_YELLOW << "written : " << written << std::endl;
	std::cout << COLOR_BOLD_YELLOW << "total_written : " << total_written << std::endl;
	std::cout << COLOR_BOLD_YELLOW << "to_write : " << to_write << "\n" << std::endl;

	while ( to_write != 0 )
	{
		fcntl( stdin_pipefd[1], F_SETFL, O_NONBLOCK );
		written = write( stdin_pipefd[1], body.c_str() + total_written , to_write );
		if (written == -1)
			return ;

		total_written += written;
   		to_write -= written;

		std::cout << COLOR_BOLD_YELLOW << "written : " << written << std::endl;
		std::cout << COLOR_BOLD_YELLOW << "total_written : " << total_written << std::endl;
		std::cout << COLOR_BOLD_YELLOW << "to_write : " << to_write << "\n" << std::endl;
	}
}
