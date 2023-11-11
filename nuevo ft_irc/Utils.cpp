/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruiz-mo <aruiz-mo@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 10:52:58 by aruiz-mo          #+#    #+#             */
/*   Updated: 2023/11/11 12:29:27 by aruiz-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"

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

std::string getPassMsg(std::string msg)
{
    size_t		posIni;
	size_t		posEnd;
	std::string	subStrAux;
	std::string	pass;

	posIni = msg.find("PASS ") + 5;
	subStrAux = msg.substr(posIni);
	posEnd = subStrAux.find("\r");
	pass = subStrAux.substr(0, posEnd);
	return (pass);
}

std::string getUserMsg(std::string msg)
{
	size_t		posIni;
	size_t		posEnd;
	std::string	subStrAux;
	std::string	user;

	posIni = msg.find("USER ") + 5;
	subStrAux = msg.substr(posIni);
	posEnd = subStrAux.find(" localhost :");
	user = subStrAux.substr(0, posEnd/2);
	return (user);
}

std::string getRealNameMsg(std::string msg)
{
	size_t		posIni;
	std::string	realName;

	posIni = msg.find(":") + 1;
	realName = msg.substr(posIni);
	return (realName);
}

std::string getNickMsg(std::string msg)
{
	size_t		posIni;
	size_t		posEnd;
	std::string	subStrAux;
	std::string	nick;

	posIni = msg.find("NICK ") + 5;
	subStrAux = msg.substr(posIni);
	posEnd = subStrAux.find('\r');
	nick = subStrAux.substr(0, posEnd);
	return (nick);
}
