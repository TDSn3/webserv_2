/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exceptions.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 12:29:56 by yfoucade          #+#    #+#             */
/*   Updated: 2023/09/12 11:15:17 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

FileOpeningException::FileOpeningException( std::string what_arg ):
_explanation(what_arg){}

const char* FileOpeningException::what( void ) const throw()
{
	return _explanation.c_str();
}

FileOpeningException::~FileOpeningException( void ) throw()
{}

ParsingError::ParsingError( std::string line, std::string what_arg ):
_line(line),
_explanation(what_arg),
_what_str("Error in line: " + _line + "\n" + _explanation){}

const char* ParsingError::what( void ) const throw()
{
	return _what_str.c_str();
}

ParsingError::~ParsingError( void ) throw()
{}