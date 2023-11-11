/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruiz-mo <aruiz-mo@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 09:22:45 by aruiz-mo          #+#    #+#             */
/*   Updated: 2023/11/10 18:29:12 by aruiz-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int main(int argc, char **argv)
{
	int			port;
	std::string	pass;

	if (argc != 3)
	{
		std::cerr << "Usage: ./ircserv port password" << std::endl;
		exit(EXIT_FAILURE);
	}
	port = std::atoi(argv[1]);
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
