/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InitSocket.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruiz-mo <aruiz-mo@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 10:09:11 by aruiz-mo          #+#    #+#             */
/*   Updated: 2023/10/30 17:26:32 by aruiz-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Ircserv.hpp"

void	init_server(int *serverSocket, int port)
{
	struct sockaddr_in	serverAddr;
	
	*serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (*serverSocket == -1) {
        std::cerr << "Error al crear el socket" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // Configurar el servidor
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(port);

    // Enlazar el socket
    if (bind(*serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error: Address already in use" << std::endl;
        close(*serverSocket);
        std::exit(EXIT_FAILURE);
    }

    // Configurar el socket como no bloqueante
    setNonBlocking(*serverSocket);
}
