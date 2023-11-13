/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcUtils.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruiz-mo <aruiz-mo@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 10:12:35 by aruiz-mo          #+#    #+#             */
/*   Updated: 2023/11/10 15:37:51 by aruiz-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServ.hpp"

int	setNonBlocking(int sockfd)
{
	return (fcntl(sockfd, F_SETFL, O_NONBLOCK));
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

void	connection(std::string mensg, std::string pass, int *sockfd, std::map<int, User *> *clients)
{
	size_t	posIni;
	size_t	posEnd;
	bool	badNick = false;
	std::string menssageOut;
	std::string subStrAux;

	posIni = mensg.find("PASS ") + 5;
	subStrAux = mensg.substr(posIni);
	menssageOut = ((*clients)[*sockfd])->getHostname() + ":" + std::to_string(((*clients)[*sockfd])->getPort()) + " has connected.";
	logMessage(menssageOut);
	if (posIni != std::string::npos)
	{
		posEnd = subStrAux.find('\r');
		if (subStrAux.substr(0, posEnd) == pass)
		{
			std::string	nick = getNickMsg(mensg);
			std::string	user = getUserMsg(mensg);
			std::string	realName = getRealNameMsg(mensg);
			//comprobar nick y si es el mismo expulsar
			for (std::map<int, User *>::iterator it = (*clients).begin(); it != (*clients).end(); ++it) {
				if (nick == it->second->getNick())
				{
					badNick = true;
					break ;
				}
			}
			if (!badNick)
			{
				(*clients)[*sockfd]->setNick(nick);
				(*clients)[*sockfd]->setUser(user);
				(*clients)[*sockfd]->setRealName(realName);
				menssageOut = ((*clients)[*sockfd])->getHostname() + ":" + std::to_string(((*clients)[*sockfd])->getPort()) + " is now known as " + nick + ".";
				logMessage(menssageOut);
				return ;
			}
		}
	}
	User* user = (*clients)[*sockfd];
	close(*sockfd);
	if (!badNick)
		menssageOut = ((*clients)[*sockfd])->getHostname() + ":" + std::to_string(((*clients)[*sockfd])->getPort()) + " has disconnected error password!";
	delete user;
	clients->erase(*sockfd);
	if (badNick)
		menssageOut = ((*clients)[*sockfd])->getHostname() + ":" + std::to_string(((*clients)[*sockfd])->getPort()) + " has disconnected error same nick!";
	logMessage (menssageOut);
}
