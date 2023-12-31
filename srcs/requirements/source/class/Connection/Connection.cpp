/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfoucade <yfoucade@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 12:11:02 by yfoucade          #+#    #+#             */
/*   Updated: 2023/11/02 17:42:13 by yfoucade         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

Connection::Connection( void ):
response_status(false),
_origin(Origin(DEFAULT_HOST, DEFAULT_PORT)),
_socket(-1),
_close(false),
_continue_reading(true)
{}
	
Connection::Connection( Origin origin, int socket ):
response_status(false),
_origin(origin),
_socket(socket),
_close(false),
_continue_reading(true)
{}
	
Connection::Connection( const Connection& other )
{
	*this = other;
}
	
Connection& Connection::operator=( const Connection& other )
{
	if ( this != &other )
	{
		response_status = other.response_status;
		response = other.response;
		_origin = other._origin;
		_socket = other._socket;
		_request = other._request;
		_close = other._close;
		_continue_reading = other._continue_reading;
		_parse_buffer = other._parse_buffer;
		_parse_buffer = other._parse_buffer;
		_line_end = other._line_end;
	}
	return *this;
}

Origin& Connection::get_origin( void )
{
	return _origin;
}

int	Connection::get_socket( void ) const
{
	return _socket;
}

bool Connection::get_close( void ) const
{
	return _close;
}

void Connection::update_close( void )
{
	return;
}

void	Connection::set_close( bool value )
{
	_close = value;
}

void Connection::close_connection( void )
{
	if ( _socket == -1 )
		return ;
	shutdown(_socket, SHUT_RDWR);
	close( _socket );
	_socket = -1;
}

void Connection::receive( void )
{
	ssize_t	ret = recv(_socket, _read_buffer, BUFF_SIZE, 0);

	if (ret > 0)
		_read_buffer[ret] = 0;
	else	// read error or connection closed by client -> close connection
	{
		std::cout << COLOR_RED << "Déconnexion de [" << _origin.get_host() << ":" << _origin.get_port() << "]" << COLOR_RESET << "\n" << std::endl;
		_close = true;
		_continue_reading = false;
	}

	if ( _continue_reading)
	{
		std::string tmp(_read_buffer, ret);
		_parse_buffer += tmp;
	}

	while ( !_request.is_complete() && _parse_buffer.size() )
	{
		std::string::size_type required_size = _request.get_required_size();
		if ( required_size == LINE )
		{
			_line_end = _parse_buffer.find("\r\n");
			if ( _line_end == std::string::npos )
				break;
			std::string line = _parse_buffer.substr(0, _line_end);
			_parse_buffer = _parse_buffer.erase(0, _line_end + 2);
			_request.add_data(line);
		}
		else
		{
			if ( _parse_buffer.size() < required_size )
				break;
			std::string line = _parse_buffer.substr(0, required_size);
			_parse_buffer = _parse_buffer.erase(0, required_size);
			_request.add_data(line);
		}
		if (_request.get_parsing_error())
			_continue_reading = false;
		if (_request.is_complete() && _request.has_close_option())
			_continue_reading = false;
	}

	if ( !_continue_reading )
		_parse_buffer.erase();
}

bool Connection::is_ready_for_reply( void )
{
	if ( _request.get_parsing_status() == done || _request.get_parsing_status() == expect_100_continue )
		return true;
	return false;
}

Request& Connection::get_request( void )
{
	return _request;
}

// void Connection::set_parse_buffer( std::string new_parse_buffer )
// {
// 	_parse_buffer = new_parse_buffer;
// }

void Connection::flush_request( void )
{
	_request = Request();
}
