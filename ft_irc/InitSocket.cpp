/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InitSocket.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruiz-mo <aruiz-mo@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 10:09:11 by aruiz-mo          #+#    #+#             */
/*   Updated: 2023/11/07 18:46:40 by aruiz-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServ.hpp"

void	init_server(int *serverSocket, int port)
{
	struct sockaddr_in	serverAddr;
	
	*serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (*serverSocket == -1) {
        std::cerr << "Error: Create socket" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(port);

    if (bind(*serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error: Address already in use" << std::endl;
        close(*serverSocket);
        std::exit(EXIT_FAILURE);
    }

    setNonBlocking(*serverSocket);
}

void init_program(int *serverSocket, std::string pass)
{
	int				kq;
	struct kevent	event;
	struct kevent	events[MAX_EVENTS];
	std::map<int, std::string> clients;

	kq = kqueue();
    if (kq == -1) {
        perror("Error: create Kqueue");
        close(*serverSocket);
        exit(EXIT_FAILURE);
    }
    EV_SET(&event, *serverSocket, EVFILT_READ, EV_ADD, 0, 0, NULL);
    kevent(kq, &event, 1, NULL, 0, NULL);
	while (true) 
	{
		int numEvents = kevent(kq, NULL, 0, events, MAX_EVENTS, NULL);
		for (int i = 0; i < numEvents; ++i) {
            int sockfd = events[i].ident;
			if (sockfd == *serverSocket) {
                struct sockaddr_in6 clientAddr;
                socklen_t clientAddrLen = sizeof(clientAddr);
                int clientSocket = accept(*serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
                setNonBlocking(clientSocket);
                EV_SET(&event, clientSocket, EVFILT_READ, EV_ADD, 0, 0, NULL);
                kevent(kq, &event, 1, NULL, 0, NULL);
				char clientIP[INET6_ADDRSTRLEN];
				inet_ntop(AF_INET6, &(clientAddr.sin6_addr), clientIP, sizeof(clientIP));
				clients[clientSocket] = clientIP;
            } else {
                char buffer[1024];
				memset(buffer, '\0', sizeof(buffer));
                ssize_t bytesRead = recv(sockfd, buffer, sizeof(buffer), 0);
                if (bytesRead <= 0) {
                    // Cliente desconectado o error de lectura
                    close(sockfd);
                    std::cout << "Cliente desconectado desde " << clients[sockfd] << std::endl;
                    clients.erase(sockfd);
                } else {
					std::string meng = buffer;
					if (meng.find("CAP LS") != std::string::npos) {
						connection(meng, pass, &sockfd, &clients);
					} else {
						//ircOptions(meng, &clients);
						std::cout << meng << std::endl;
					}
				}
			}
		}
	}
}
