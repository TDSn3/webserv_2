/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StatusCode.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 16:13:31 by tda-silv          #+#    #+#             */
/*   Updated: 2023/11/01 09:52:52 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STATUSCODE_HPP
# define STATUSCODE_HPP

# include "../Server.hpp"
# include "../HttpResponse.hpp"
# include "../Connection.hpp"

class StatusCode : public std::exception
{
	public:

	StatusCode(int code) : status_code(code) {};
	StatusCode(const StatusCode &src) : status_code(src.status_code) {};

	void	build_response_error( Connection &connection ) const
	{
		connection.response.clear();		
		connection.response.build_error(connection.get_request(), status_code);
	};

	void	build_response_error( Connection &connection, Server &server ) const
	{
		connection.response.clear();		
		connection.response.build_error(connection.get_request(), server, status_code);
	};

	const int	status_code;

	protected:

	private:

};

#endif
