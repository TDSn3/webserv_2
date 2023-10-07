/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_connections.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 10:34:21 by tda-silv          #+#    #+#             */
/*   Updated: 2023/10/06 10:12:21 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

static bool		give_and_check_index(std::vector<pollfd> &poll_struct, int fd, size_t &index);
static ssize_t	give_index_fd_in_poll_struct(std::vector<pollfd> &poll_struct, int fd);

void	Gateway::close_connections( void )
{
	connection_iter_type	connection_iter = _connections.begin();
	size_t					index;

	while ( connection_iter != _connections.end() )
	{
		connection_iter->update_close();
		if ( connection_iter->get_close() )
		{
			if (give_and_check_index(poll_struct, connection_iter->get_socket(), index) == false)
				continue ;

			connection_iter->close_connection();
			connection_iter = _connections.erase( connection_iter );
			poll_struct.erase( poll_struct.begin() + static_cast<long>( index ) );
		}
		else
			++connection_iter;
	}
}

static bool	give_and_check_index(std::vector<pollfd> &poll_struct, int fd, size_t &index)
{
	ssize_t	index_check;
	
	index_check = give_index_fd_in_poll_struct(poll_struct, fd);
	if (index_check == -1)
	{
		std::cout << COLOR_BOLD_MAGENTA << "STRANGE !" << COLOR_RESET << std::endl;	// TODO: à supprimer à la fin
		return (false);
	}
	index = static_cast<size_t>(index_check);
	return (true);
}

static ssize_t	give_index_fd_in_poll_struct(std::vector<pollfd> &poll_struct, int fd)
{
	size_t	i;

	i = 0;
	while (i < poll_struct.size() )
	{
		if (poll_struct[i].fd == fd)
			return (static_cast<ssize_t>(i) );
		i++;
	}
	return (-1);
}
