/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StatusCode.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 16:13:31 by tda-silv          #+#    #+#             */
/*   Updated: 2023/09/13 15:52:23 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STATUSCODE_HPP
# define STATUSCODE_HPP

# include "HttpResponse.hpp"

class StatusCode : public std::exception
{
	public:

	StatusCode(int code) : status_code(code) {};
	StatusCode(const StatusCode &src) : status_code(src.status_code) {};

	void	build_response_error(HttpResponse &response) const
	{
		(void) response;
		// response.clear();
		// response.build_error(it->request, status_code);
		// request.clear();
	};

	const int	status_code;

	protected:

	private:

};

#endif
