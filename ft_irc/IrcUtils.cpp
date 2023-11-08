/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcUtils.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruiz-mo <aruiz-mo@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 10:12:35 by aruiz-mo          #+#    #+#             */
/*   Updated: 2023/11/02 13:01:26 by aruiz-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Ircserv.hpp"

int	setNonBlocking(int sockfd)
{
	return (fcntl(sockfd, F_SETFL, O_NONBLOCK));
}

void	connection(std::string mensg, std::string pass, int *sockfd, std::map<int, std::string> *clients)
{
	size_t	posIni;
	size_t	posEnd;
	std::string subStrAux;

	posIni = mensg.find("PASS ") + 5;
	subStrAux = mensg.substr(posIni);
	if (posIni != std::string::npos)
	{
		posEnd = subStrAux.find('\r');
		if (subStrAux.substr(0, posEnd) == pass)
		{
			mensg = subStrAux;
			posIni = mensg.find("NICK ") + 5;
			subStrAux = mensg.substr(posIni);
			posEnd = subStrAux.find('\r');
			std::cout << "NICK: " << subStrAux.substr(0, posEnd) << " -> conectado" << std::endl;
			return ;
		}
		//EN este if podemos recoger los datos del usuario (nick, user y pedir una contraseña)
	}
	close(*sockfd);
    	std::cout << "ERROR: PASSWORD." << std::endl; //464
   	clients->erase(*sockfd);
}
