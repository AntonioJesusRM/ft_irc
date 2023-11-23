/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruiz-mo <aruiz-mo@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 09:22:45 by aruiz-mo          #+#    #+#             */
/*   Updated: 2023/11/23 13:58:18 by aruiz-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

int main(int argc, char **argv)
{
	int			port;
	std::string	pass;

	if (argc != 3)
	{
		std::cerr << "Usage: ./ircserv port password" << std::endl;
		exit(EXIT_FAILURE);
	}
	for (const char* ptr = argv[1]; *ptr != '\0'; ++ptr) {
        if (!isdigit(*ptr)) {
            std::cerr << "ERROR: Port is invalid." << std::endl;
			exit(EXIT_FAILURE);
        }
    }
	port = std::atoi(argv[1]);
	if (port < 0  || port > 65535)
	{
		std::cerr << "ERROR: Port is invalid." << std::endl;
		exit(EXIT_FAILURE);
	}
	pass = argv[2];
	try
    {
		Server server(port, pass);
		server.start();
        return 0;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
