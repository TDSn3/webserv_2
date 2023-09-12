/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Origin.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfoucade <yfoucade@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/06 17:49:02 by yfoucade          #+#    #+#             */
/*   Updated: 2023/05/18 21:00:30 by yfoucade         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <utility>
#include <string>

class Origin
{
	private:
		std::pair< std::string, std::string > _host_port;

	public:
		Origin();
		Origin( const std::string&, const std::string& );
		Origin( const Origin& );
		Origin& operator=( const Origin& );
		~Origin();
		const std::string&	get_host() const;
		const std::string& get_port() const;
		void set_host( std::string );
		void set_port( std::string );
		friend bool operator<( const Origin& lhs, const Origin& rhs );
};
