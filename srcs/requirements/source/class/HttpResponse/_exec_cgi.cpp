/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _exec_cgi.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 15:19:36 by tda-silv          #+#    #+#             */
/*   Updated: 2023/09/17 12:02:37 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

static void	new_char_for_execve(Request &request, std::vector<char *> &arg_for_execve, std::string &new_path);

std::string	HttpResponse::_exec_cgi(std::string path, Request &request, char **env)	// ! throw possible
{
	std::string			new_path;
	struct stat			stat_buffer;
	int					pipefd[2];
	pid_t				pid;
	std::string			str;
	std::vector<char *>	arg_for_execve;

	if (!path.empty() && path[0] == '/')
		new_path = std::string(ROOT) + path;
	else if (!path.empty() && path[0] != '/')
		new_path = std::string(ROOT) + "/" + path;

	if (stat(new_path.c_str(), &stat_buffer) != 0)		// Vérifie si le fichier existe
		my_perror_and_throw( "cgi file does not exist", StatusCode(404) );
	if (access(new_path.c_str(), R_OK | X_OK) != 0)		// Vérifie si le fichier est accessible en lecture et en exécution
		/*my_perror_and_throw("cgi file is not readable or executable", StatusCode(403) )*/ {}

	new_char_for_execve(request, arg_for_execve, new_path);

	pipe(pipefd);										// pipefd[0] en lecture, pipefd[1] en écriture

	pid = fork();

	if (pid == 0)						// enfant
	{
		close(pipefd[0] ); 				// ferme l'extrémité en lecture
		dup2(pipefd[1], STDOUT_FILENO);	// redirige stdout vers pipefd[1]
		execve(new_path.c_str(), arg_for_execve.data(), env);
	}
	else if (pid > 0)					// parent
	{
		ssize_t	ret = 1;
		char	buffer[4096];

		close(pipefd[1] );  			// Ferme l'extrémité en écriture
		while (ret > 0)
		{
			memset(buffer, 0, sizeof(buffer) );
			ret = read(pipefd[0], buffer, sizeof(buffer) );
			str += buffer;
		}
		waitpid(pid, NULL, 0);			// TODO: ajouter une limite
	}

	for (size_t i = 0; i < arg_for_execve.size() -1 ; i++)
		delete [] arg_for_execve[i];
	
	return (str);
}

static void	new_char_for_execve(Request &request, std::vector<char *> &arg_for_execve, std::string &new_path)
{
	char	*str;

	str = new char[new_path.size() + 1];
	std::strcpy(str, new_path.c_str() );
	arg_for_execve.push_back(str);

	for (std::map<std::string, std::string> :: iterator it = request.request_line.parsed_url.query_parameters.begin();
		it != request.request_line.parsed_url.query_parameters.end();
		it++)
	{
		str = new char[it->first.size() + it->second.size() + 1];
		std::strcpy(str, (it->first + it->second).c_str() );
		arg_for_execve.push_back(str);
	}
	arg_for_execve.push_back(NULL);
}
