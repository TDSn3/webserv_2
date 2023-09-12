/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exceptions.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfoucade <yfoucade@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 11:38:28 by yfoucade          #+#    #+#             */
/*   Updated: 2023/07/26 05:37:07 by yfoucade         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <exception>
#include <string>

class FileOpeningException: public std::exception
{
	private:
		std::string _explanation;

	public:
		FileOpeningException( std::string what_arg );
		~FileOpeningException( void ) throw();
		virtual const char* what( void ) const throw();
};

class ParsingError: public std::exception
{
	private:
		std::string _line;
		std::string _explanation;
		std::string _what_str;
	
	public:
		ParsingError( std::string line, std::string what_arg );
		~ParsingError( void ) throw();
		virtual const char* what( void ) const throw();
};
