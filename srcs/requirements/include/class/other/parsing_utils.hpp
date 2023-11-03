/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfoucade <yfoucade@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 02:17:04 by yfoucade          #+#    #+#             */
/*   Updated: 2023/11/02 13:26:58 by yfoucade         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <vector>

// std::string ALPHA = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
// std::string DIGIT = "0123456789";
// std::string HEXDIGIT = DIGIT + "ABCDEFabcdef";
// std::string TCHAR = "!#$%&'*+-.^_`|~" + DIGIT + ALPHA;
// std::string UNRESERVED = ALPHA + DIGIT + "-._~";
// std::string SUB_DELIMS = "!$&'()*+,;=";

bool is_whitespace( char c );
bool is_in_charlist(char c, std::string set);
bool	is_uri( std::string str );
bool	is_simple_directive( std::vector< std::string > tokens );
std::vector< std::string > tokenize( std::string str );
std::vector< std::string > tokenize_nows( std::string str );

std::vector< std::string >::iterator skip_whitespaces_tokens(
	std::vector< std::string >::iterator,
	std::vector< std::string >::iterator);

bool	is_alpha( char c );
bool	is_digit( char c );
bool	is_digit( const char* str );
bool	is_hexdigit( char c );
bool	is_tchar( char c );
bool	is_unreserved( char c );
bool	is_sub_delims( char c );
bool	is_pchar( std::string::const_iterator, std::string::const_iterator );
bool	is_vchar( char c );
bool	is_field_vchar( unsigned char c );

void	ft_tolower( std::string& );
void	trim( std::string& );

std::string	ft_tolower_str( std::string str );
