/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ComunicationUtils.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruiz-mo <aruiz-mo@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 10:52:58 by aruiz-mo          #+#    #+#             */
/*   Updated: 2023/11/10 13:53:43 by aruiz-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServ.hpp"

void logMessage(const std::string& message)
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

void    clientMessage(const std::string& message, int fd)
{
    std::string buffer = message + "\r\n";
    if (send(fd, buffer.c_str(), buffer.length(), 0) < 0)
        std::cerr << "Error send message." << std::endl;
}