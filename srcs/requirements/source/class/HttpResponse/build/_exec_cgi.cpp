/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _exec_cgi.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfoucade <yfoucade@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 13:08:11 by yfoucade          #+#    #+#             */
/*   Updated: 2023/11/03 13:08:12 by yfoucade         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <header.hpp>

static void			check_file( std::string &path );
static void			new_char_for_execve( Request &request, std::vector<char *> &arg_for_execve, std::string &path );
static void			env_update_push_back( std::vector<char *> &env_update, const char *str_to_add );
static void			new_char_for_env_update( std::vector<char *> &env_update, char **env, Request &request, std::string path, std::string path_target, Server &server );
static void			fork_child( Gateway &gateway, std::string cgi_input_path, std::string cgi_output_path, std::vector<char *> &arg_for_execve, std::vector<char *> &env_update );
static void			fork_parent( int pid );
static void			read_file_stock_output( std::string cgi_output_path, std::string &str );
static std::string	to_upper_str( std::string str );
static std::string	dash_to_underscore( std::string str );
static void			parse_cgi_output2( std::string &str, std::string &cgi_output );

std::string	HttpResponse::_exec_cgi( Gateway &gateway, std::string &path, std::string &path_target, Request &request, char **env, Server &server )	// ! throw possible
{
	int					cgi_input_fd;
	pid_t				pid;
	std::string			cgi_input_path(".TEMP_IN");
	std::string			cgi_output_path(".TEMP_OUT");
	std::string			ret;
	std::string			cgi_output;
	std::vector<char *>	arg_for_execve;
	std::vector<char *>	env_update;

	try
	{
		check_file( path );	// ! throw possible

		new_char_for_execve( request, arg_for_execve, path );
		new_char_for_env_update( env_update, env, request, path, path_target, server );
		cgi_input_fd = open( cgi_input_path.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0666 );
		write( cgi_input_fd, request.get_body().c_str(), request.get_body().size() );
		close( cgi_input_fd );
		pid = fork();

		if ( pid == 0 )
			fork_child( gateway, cgi_input_path, cgi_output_path, arg_for_execve, env_update );
		else if (pid > 0)
			fork_parent( pid );

		read_file_stock_output( cgi_output_path, cgi_output );

		for ( size_t i = 0; i < arg_for_execve.size() -1 ; i++ )
			delete [] arg_for_execve[i];

		for ( size_t i = 0; i < env_update.size() -1 ; i++ )
			delete [] env_update[i];

		// parse_cgi_output( ret, request.get_body() );
		parse_cgi_output2( ret, cgi_output );

		return ( ret );

	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		
		for ( size_t i = 0; i < arg_for_execve.size(); i++ )
			delete [] arg_for_execve[i];

		for ( size_t i = 0; i < env_update.size(); i++ )
			delete [] env_update[i];

		throw;
	}
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
	
	(void)server;
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

	// path_target = path_target.substr( server.root.size() + 1 );

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
	env_update_push_back( env_update, ( "REQUEST_URI=" + path_target ).c_str() );
	// env_update_push_back( env_update, "SERVER_NAME=127.0.0.1" );					// optionnel
	// env_update_push_back( env_update, "SERVER_PORT=8080" );						// optionnel
	env_update_push_back( env_update, "SERVER_PROTOCOL=HTTP/1.1" );

	env_update.push_back( NULL );
}

static void fork_child( Gateway &gateway, std::string input_path, std::string output_path, std::vector<char *> &arg_for_execve, std::vector<char *> &env_update )
{
	int input_fd = open( input_path.c_str(), O_RDONLY | O_CREAT, 0666 );
	int output_fd = open( output_path.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0666 );
	if ( input_fd == -1 || output_fd == -1 )
	{
		// todo: throw a ChildProcessException and free arg_for_execve and env_update,
		// the exception will eventually be catched by Gateway,
		// which will free memory and exit with status 1.
		// parent process catches exit status and knows what to do.
		// OR
		// Set the global variable sigint_signal to some value
		// do not read/write anything if sigint_signal is set to that value
		// listen loop will test for that value and break.
		// OR
		// in calling function (_exec_cgi), free arg_for_execve and env_update,
		// then exit( FAILURE ); and use on_exit() to free memory. (forbidden)
		// OR
		// create Gateway (or a pointer to Gateway) as a global variable,
		// free everything from here and exit( FAILURE ).
		// OR
		// pass the Gateway to each 
		;
	}
	dup2( input_fd, STDIN_FILENO );
	dup2( output_fd, STDOUT_FILENO );
	execve( arg_for_execve[0], arg_for_execve.data(), env_update.data() );
	gateway.~Gateway();
	// todo: throw same exception here
}

static void fork_parent( int pid )
{
	// todo:
	// in a loop, waitpid with w_nohang option and keep track of elapsed time,
	// after some time, kill child and throw exception.
	int		waitpid_return_value = 0;
	time_t	tic, toc;
	int		cgi_exit_status;

	tic = time( NULL );
	while ( !waitpid_return_value )
	{
		waitpid_return_value = waitpid(pid, &cgi_exit_status, WNOHANG);
		toc = time( NULL );
		if ( difftime(toc, tic) > 10 )
			kill(pid, SIGINT);
	}
	if ( waitpid_return_value == -1 || !WIFEXITED(cgi_exit_status) || WEXITSTATUS(cgi_exit_status) )
		my_perror_and_throw( "Error", StatusCode( 500 ) );
	return;

}

static void	read_file_stock_output( std::string cgi_output_path, std::string &str )
{
	char	buffer[4096];
	ssize_t	ret;

	int cgi_output_fd = open( cgi_output_path.c_str(), O_RDWR, 0666 );
	if ( cgi_output_fd == -1 )
		my_perror_and_throw( "Error opening CGI's output file", StatusCode( 500 ) );
		
	lseek( cgi_output_fd, 0, SEEK_SET );	// Réinitialise le pointeur du fd
	ret = read( cgi_output_fd, buffer, sizeof( buffer ) - 1 );
	while ( ret > 0 )
	{
		str.append( buffer, ret );
		ret = read( cgi_output_fd, buffer, sizeof( buffer ) - 1 );
	}
	
	close( cgi_output_fd );
	if ( ret == -1 )
		my_perror_and_throw( "Internal Server Error", 500 );
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

static void	parse_cgi_output2( std::string &str, std::string &cgi_output )
{
	str = CGIResponse( cgi_output ).parsed_response;
}
