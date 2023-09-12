/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LogFile.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 12:26:30 by tda-silv          #+#    #+#             */
/*   Updated: 2023/08/23 15:47:55 by tda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGFILE_HPP
# define LOGFILE_HPP

# include <iostream>
# include <sstream>
# include <fstream>

class LogFile
{
	public:

		LogFile(void)
		{
			_cout_buffer = std::cout.rdbuf();		// sauvegarde le tampon (buffer) de sortie pour le réaffecter ultérieurement
			_cerr_buffer = std::cerr.rdbuf();		//

			_log_file.open("log");					// redirige cout et cerr vers le buffer du fichier log
													//
			std::cout.rdbuf(_log_file.rdbuf() );	//
			std::cerr.rdbuf(_log_file.rdbuf() );	//	
		};

		~LogFile(void)
		{
			std::cout.rdbuf(_cout_buffer);
			std::cerr.rdbuf(_cerr_buffer);
			_log_file.close();	
		};

	protected:

	private:

		std::streambuf* _cout_buffer;
		std::streambuf* _cerr_buffer;
		std::ofstream	_log_file;

};

#endif
