/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcUtils.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruiz-mo <aruiz-mo@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 10:12:35 by aruiz-mo          #+#    #+#             */
/*   Updated: 2023/11/07 18:46:59 by aruiz-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServ.hpp"

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
			posIni = mensg.find("NICK ") + 5;
			subStrAux = mensg.substr(posIni);
			posEnd = subStrAux.find('\r');
			//comprobar nick y si es el mismo expulsar
			std::cout << "User " << subStrAux.substr(0, posEnd) << " connected." << std::endl;
			return ;
		}
	}
	close(*sockfd);
    std::cout << "ERROR: PASSWORD." << std::endl;
   	clients->erase(*sockfd);
}
