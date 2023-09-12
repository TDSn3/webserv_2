/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Origin.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/06 17:57:27 by yfoucade          #+#    #+#             */
/*   Updated: 2023/09/12 11:23:42 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

Origin::Origin( void ):
_host_port( std::make_pair(DEFAULT_HOST, DEFAULT_PORT) ){}

Origin::Origin( const std::string& host, const std::string& port):
_host_port( std::make_pair(host, port) ){}

Origin::Origin( const Origin& other )
{
	_host_port = std::make_pair(other.get_host(), other.get_port());
	// _host_port = other._host_port;
}

Origin& Origin::operator=( const Origin& other )
{
	if ( this == &other )
		return *this;
	_host_port = std::make_pair(other.get_host(), other.get_port());
	// _host_port = other._host_port;
	return *this;
}

Origin::~Origin(){}

const std::string& Origin::get_host( void ) const
{
	return _host_port.first;
}

const std::string& Origin::get_port( void ) const
{
	return _host_port.second;
}

void Origin::set_host( std::string host )
{
	_host_port.first = host;
}

void Origin::set_port( std::string port )
{
	_host_port.second = port;
}

bool operator<( const Origin& lhs, const Origin& rhs )
{
	return lhs._host_port < rhs._host_port;
}
