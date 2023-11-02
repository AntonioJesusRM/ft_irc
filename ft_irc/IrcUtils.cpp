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
	int	flags;
	flags = fcntl(sockfd, F_GETFL, 0);
    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
    return 0;
}

void	connection(std::string mensg, std::string pass, int *sockfd, std::map<int, std::string> *clients)
{
	size_t	posIni;
	size_t	posEnd;

	posIni = mensg.find("PASS ") + 5;
	if (posIni != std::string::npos)
	{
		posEnd = mensg.find('\r');
		if (mensg.substr(posIni, posEnd - posIni) == pass)
			return ;
		//EN este if podemos recoger los datos del usuario (nick, user y pedir una contraseña)
	}
	close(*sockfd);
    std::cout << "ERROR: PASSWORD." << std::endl;
    clients->erase(*sockfd);
}