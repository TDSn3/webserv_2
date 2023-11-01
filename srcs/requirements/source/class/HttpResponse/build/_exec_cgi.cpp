/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _exec_cgi.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfoucade <yfoucade@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 15:19:36 by tda-silv          #+#    #+#             */
/*   Updated: 2023/11/01 23:13:23 by yfoucade         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

static void			check_file( std::string &path );
static void			new_char_for_execve( Request &request, std::vector<char *> &arg_for_execve, std::string &path );
static void			env_update_push_back( std::vector<char *> &env_update, const char *str_to_add );
static void			new_char_for_env_update( std::vector<char *> &env_update, char **env, Request &request, std::string path, std::string path_target, Server &server );
static void			fork_child( int stdin_pipefd[2], int file_stock_output_fd, std::vector<char *> &arg_for_execve, std::vector<char *> &env_update );
static void			fork_parent( int stdin_pipefd[2], std::string &str, int pid );
static void			read_file_stock_output( int file_stock_output_fd, std::string &str );
static std::string	to_lower_str( std::string str );
static std::string	to_upper_str( std::string str );
static std::string	dash_to_underscore( std::string str );
static void			parse_cgi_output( std::string &str, std::string &body );

std::string	HttpResponse::_exec_cgi( std::string &path, std::string &path_target, Request &request, char **env, Server &server )	// ! throw possible
{
	int					file_stock_output_fd;
	int					stdin_pipefd[2];
	pid_t				pid;
	std::string			file_stock_output_path(".TEMP");
	std::string			ret;
	std::vector<char *>	arg_for_execve;
	std::vector<char *>	env_update;

	std::cout << "++++++++++++> " << path << "\n";
	std::cout << "++++++++++++> " << path_target << "\n";
	// path_target = path_target.substr( server.root.size() + 1 );
	// std::cout << "============> " << path_target << "\n";

	// TODO: check failure of open
	file_stock_output_fd = open( file_stock_output_path.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0666 );

	check_file( path );	// ! throw possible

	// TODO: handle failure of new[]
	new_char_for_execve( request, arg_for_execve, path );
	new_char_for_env_update( env_update, env, request, path, path_target, server );

	pipe( stdin_pipefd );

	pid = fork();

	if ( pid == 0 )
		fork_child( stdin_pipefd, file_stock_output_fd, arg_for_execve, env_update );
	else if (pid > 0)
		fork_parent( stdin_pipefd, request.get_body(), pid ); // TODO: if FAILURE, close file_stock_output_fd

	// TODO: if not failure of fork_parent, do:
	read_file_stock_output( file_stock_output_fd, ret );
	// TODO: delete vectors content (see below),
	// then if FAILURE of read_file_stock_output, set 'ret' to empty string and return

	for ( size_t i = 0; i < arg_for_execve.size() -1 ; i++ )
		delete [] arg_for_execve[i];

	for ( size_t i = 0; i < env_update.size() -1 ; i++ )
		delete [] env_update[i];

	parse_cgi_output( ret, request.get_body() );

	return ( ret );

}

static void	check_file( std::string &path )				// ! throw possible
{
	struct stat			stat_buffer;
	
	if ( stat( path.c_str(), &stat_buffer ) != 0 )		// Vérifie si le fichier existe
		my_perror_and_throw( "cgi file does not exist", StatusCode( 404 ) );
	if ( access( path.c_str(), R_OK | X_OK ) != 0 )		// Vérifie si le fichier est accessible en lecture et en exécution
		my_perror_and_throw( "cgi file is not readable or executable", StatusCode( 403 ) );
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

static void	env_update_push_back( std::vector<char *> &env_update, const char *str_to_add )
{
	char	*str;
		
	str = new char[ std::string( str_to_add ).size() + 1 ];
	std::strcpy( str, str_to_add );
	env_update.push_back( str );
}

static void	new_char_for_env_update( std::vector<char *> &env_update, char **env, Request &request, std::string path, std::string path_target, Server &server )
{
	std::map< std::string, std::vector< std::string > > :: iterator	it = request._header_section.begin();
	std::string														str;
	std::ostringstream												oss;
	
	for ( size_t i = 0; env[i]; i++ )
		env_update_push_back( env_update, env[i] );

	while ( it != request._header_section.end() )
	{
		str = "HTTP_" + to_upper_str( dash_to_underscore( it->first ) ) + "=";
		for ( std::vector< std::string > :: iterator it2 = it->second.begin(); it2 != it->second.end(); it2++ )
				str += *it2;
		if ( it->first != "host" )
			env_update_push_back( env_update, str.c_str() );
		it++;
	}

	oss << request.get_body().size();
	env_update_push_back( env_update, ( "CONTENT_LENGTH=" + oss.str() ).c_str() );

	env_update_push_back( env_update, ( "REQUEST_METHOD=" + request.request_line.method ).c_str() );

	path_target = path_target.substr( server.root.size() + 1 );

	env_update_push_back( env_update, ( "SCRIPT_NAME=" + path ).c_str() );

	str = "QUERY_STRING=";
	for ( std::map<std::string, std::string> :: iterator it2 = request.request_line.parsed_url.query_parameters.begin();
		it2 != request.request_line.parsed_url.query_parameters.end();
		it2++ )
	{
		str += it2->first + "=" + it2->second;
	}
	env_update_push_back( env_update, str.c_str() );

	// std::cout << COLOR_BOLD_MAGENTA << request.request_line.parsed_url.path << COLOR_RESET << "\n";

	// env_update_push_back( env_update, "GATEWAY_INTERFACE=CGI/1.1" );				// optionnel
	
	(void) path_target;
	env_update_push_back( env_update, ( "PATH_INFO=" + path_target ).c_str() );
	// env_update_push_back( env_update, "PATH_INFO=YoupiBanane/youpi.bla" );

	// env_update_push_back( env_update, "PATH_TRANSLATED=YoupiBanane/youpi.bla" );	// optionnel
	// env_update_push_back( env_update, "REQUEST_URI=YoupiBanane/youpi.bla" );		//
	env_update_push_back( env_update, ("REQUEST_URI=" + path_target ).c_str() );
	// env_update_push_back( env_update, "SERVER_NAME=127.0.0.1" );					// optionnel
	// env_update_push_back( env_update, "SERVER_PORT=8080" );						// optionnel
	env_update_push_back( env_update, "SERVER_PROTOCOL=HTTP/1.1" );

	env_update.push_back( NULL );
}

static void fork_child( int stdin_pipefd[2], int file_stock_output_fd, std::vector<char *> &arg_for_execve, std::vector<char *> &env_update )
{
	dup2( stdin_pipefd[0], STDIN_FILENO );
	close( stdin_pipefd[1] );
	dup2( file_stock_output_fd, STDOUT_FILENO );
	execve( arg_for_execve[0], arg_for_execve.data(), env_update.data() );
}

static void fork_parent( int stdin_pipefd[2], std::string &str, int pid )
{
	close( stdin_pipefd[0] );
	write( stdin_pipefd[1], str.c_str(), str.size() ); // TODO: cannot write without going through poll. Use regular file
	close( stdin_pipefd[1] );
	waitpid(pid, 0, 0); // TODO: add timeout and if timeout kill and return FAILURE 
}

static void	read_file_stock_output( int file_stock_output_fd, std::string &str )
{
	char	buffer[4096];
	ssize_t	ret;

	ret = 1;
	lseek( file_stock_output_fd, 0, SEEK_SET );	// Réinitialise le pointeur du fd
	while ( ret > 0 )
	{
		ret = read( file_stock_output_fd, buffer, sizeof( buffer ) );
		str.append( buffer, ret );
	}

	// TODO
	// ret = read( file_stock_output_fd, buffer, sizeof( buffer ) );
	// while ( ret > 0 )
	// {
	// 	str.append( buffer, ret );
	// 	ret = read( file_stock_output_fd, buffer, sizeof( buffer ) );
	// }
	// close( file_stock_output_fd );
	// if ( ret == -1 )
	// 	return ( FAILURE );
	// return ( SUCCESS );

	close( file_stock_output_fd );
}

static std::string to_lower_str( std::string str )
{
	std::string	ret;

	for ( size_t i = 0; str[i]; i++ )
		ret += std::tolower( static_cast<unsigned char> ( str[i] ) ) ;
	return ( ret );
}

static std::string to_upper_str( std::string str )
{
	std::string	ret;

	for ( size_t i = 0; str[i]; i++ )
		ret += std::toupper( static_cast<unsigned char> ( str[i] ) ) ;
	return ( ret );
}

static std::string dash_to_underscore( std::string str )
{
	std::string	ret;

	for ( size_t i = 0; str[i]; i++ )
		str[i] == '-' ? ( ret += "_" ) : ( ret += str[i] );
	return ( ret );
}

static void	parse_cgi_output( std::string &str, std::string &body )
{
	std::string			header_update;
	std::ostringstream	oss;

	size_t	start = 0;
	for ( size_t i = 0; str[i]; i++ )
	{
		if ( str[i] == '\n' && str[i + 1] && ( str[i + 1] == '\n' || ( str[i + 1] == '\r' && str[i + 2] && str[i + 2] == '\n' ) ) )
		{
			oss << body.size();
			str.insert( i + 1, "Content-Length: " + oss.str() + "\r\n" );
			return ;
		}
		if ( i > 0 && str[i - 1] == '\n' )
			start = i;
		if ( str[i] == '\n' && i != start )	// Si je suis à la fin de la ligne et que le début de la ligne n'est pas un '\n'
		{
			if ( to_lower_str( str.substr( start, start + 7) ) == "status:" )
			{
				str.replace( start, start + 7, "HTTP/1.1" );
				i++;
			}
		}
	}
}
