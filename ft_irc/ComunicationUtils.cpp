/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ComunicationUtils.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruiz-mo <aruiz-mo@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 10:52:58 by aruiz-mo          #+#    #+#             */
/*   Updated: 2023/11/08 10:57:47 by aruiz-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Ircserv.hpp"

void log(const std::string& message)
{
	time_t      rawtime;
    struct tm   *timeinfo;
    char        buffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", timeinfo);
    std::string str(buffer);
	std::cout << "\033[0;34m[" << str << "]\033[0m ";
    std::cout << message << std::endl;
}