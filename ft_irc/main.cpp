/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruiz-mo <aruiz-mo@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 09:22:45 by aruiz-mo          #+#    #+#             */
/*   Updated: 2023/10/30 18:03:44 by aruiz-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Ircserv.hpp"

int main(int argc, char **argv)
{
	int port;
	std::string pass;
	int serverSocket;

	if (argc != 3)
	{
		std::cerr << "Usage: ./ircserv port password" << std::endl;
		exit(EXIT_FAILURE);
	}
	port = std::atoi(argv[1]);
	pass = argv[2];
	init_server(&serverSocket, port);
	close (serverSocket);
}
