/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 18:27:30 by yfoucade          #+#    #+#             */
/*   Updated: 2023/09/12 11:18:50 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <header.hpp>

bool is_whitespace( char c )
{
	return ( c == ' ' ) || ( c == '\t' );
}

bool is_in_charlist(char c, std::string set)
{
	const char *str = set.c_str();
	
	while (*str)
	{
		if (c == *str++)
			return true;
	}
	return false;
}

std::vector< std::string > tokenize( std::string str ){
	std::string::iterator token_start = str.begin();
	std::string::iterator token_end;
	std::string::iterator last = str.end();
	std::vector< std::string > res;
	
	while ( (token_start != last) && (*token_start != '#') )
	{
		token_end = token_start + 1;
		if ( (!is_whitespace(*token_start)) && (!is_in_charlist(*token_start, "{;}")) )
		{
			while ( (token_end != last) && (!is_whitespace(*token_end)) && (!is_in_charlist(*token_end, "{;}#")) )
				++token_end;
		}
		res.push_back( str.substr( token_start - str.begin(), token_end - token_start ) );
		token_start = token_end;
	}
	return res;
}

std::vector< std::string > tokenize_nows( std::string str ){
	std::string::iterator token_start = str.begin();
	std::string::iterator token_end;
	std::string::iterator last = str.end();
	std::vector< std::string > res;
	
	while ( (token_start != last) && (*token_start != '#') )
	{
		while ( (token_start != last) && (is_whitespace(*token_start)) )
			++token_start;
		if ( (token_start == last ) || (is_in_charlist(*token_start, "#")) )
			break;
		token_end = token_start + 1;
		if ( !is_in_charlist(*token_start, "{;}") )
		{
			while ( (token_end != last) && (!is_whitespace(*token_end)) && (!is_in_charlist(*token_end, "{;}#")) )
				++token_end;
		}
		res.push_back( str.substr( token_start - str.begin(), token_end - token_start ) );
		token_start = token_end;
	}
	return res;
}

std::vector< std::string >::iterator skip_whitespaces_tokens(
	std::vector< std::string >::iterator curr,
	std::vector< std::string >::iterator last)
{
	while ( (curr != last) && (is_whitespace((*curr)[0])) )
		++curr;
	return curr;
}

bool is_uri( std::string str )
{
	return str[0] == '/';
}

bool	is_simple_directive( std::vector< std::string > tokens )
{
	std::vector< std::string >::iterator first = tokens.begin();
	std::vector< std::string >::iterator last = tokens.end();
	int n_words = 0;

	while ( (first != last) && ((*first)[0] != ';') )
	{
		if (is_in_charlist((*first)[0], "{}"))
			return false;
		else
		{
			++first;
			++n_words;
		}
	}
	if ( (n_words >= 2) && ((*first)[0] == ';'))
		return true;
	return false;
}

bool	is_alpha( char c )
{
	return ( ('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z') );
}

bool	is_digit( char c )
{
	return ( '0' <= c && c <= '9' );
}

bool	is_digit( const char* str )
{
	if ( !*str )
		return false;
	while (*str)
	{
		if ( !is_digit(*str++) )
			return false;
	}
	return true;
}

bool	is_hexdigit( char c )
{
	return ( is_digit(c) || is_in_charlist(c, "ABCDEFabcdef") );
}

bool	is_tchar( char c )
{
	return ( is_digit(c) || is_alpha(c) || is_in_charlist(c, "!#$%&'*+-.^_`|~"));
}

bool	is_unreserved( char c )
{
	return ( is_digit(c) || is_alpha(c) || is_in_charlist(c, "-._~"));
}

bool	is_sub_delims( char c )
{
	return is_in_charlist(c, "!$&'()*+,;=");
}

bool	is_pchar( std::string::const_iterator first, std::string::const_iterator last )
{
	if ( first == last )
		return false;
	if (   is_unreserved(*first)
		|| is_sub_delims(*first)
		|| (*first == ':')
		|| (*first == '@') )
		return true;
	if ( *first++ != '%' )
		return false;
	if ( (first == last) || !is_hexdigit(*first++) )
		return false;
	if ( (first == last) || !is_hexdigit(*first++) )
		return false;
	return true;
}

bool	is_vchar( char c )
{
	return ( 31 <= c && c <= 126);
}

bool	is_field_vchar( unsigned char c )
{
	return ( is_vchar(c) || (128 <= c) );
}

void	ft_tolower( std::string& str )
{
	std::string::iterator it = str.begin();
	std::string::iterator end = str.end();
	
	while ( it != end )
	{
		*it =  static_cast<char>(tolower(*it) );
		++it;
	}
}

void	trim( std::string& str )
{
	std::string::iterator first = str.begin();
	std::string::iterator it, last;
	std::string::iterator end = str.end();

	while ( (first != end) && (is_whitespace(*first)) )
		++first;
	it = first;
	last = end;
	while ( it != end )
	{
		if ( !is_whitespace(*it) )
			last = it;
		++it;
	}
	str = std::string(first, last + 1);
}
