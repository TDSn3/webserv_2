/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   status.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 11:03:10 by tda-silv          #+#    #+#             */
/*   Updated: 2023/09/17 11:03:27 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

bool	HttpResponse::status(void)
{
	if (str_response.empty() )
		return (false);	// vide
	return (true);		// remplie
};
