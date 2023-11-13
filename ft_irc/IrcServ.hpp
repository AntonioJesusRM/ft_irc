/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcServ.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruiz-mo <aruiz-mo@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 09:19:29 by aruiz-mo          #+#    #+#             */
/*   Updated: 2023/11/10 13:54:32 by aruiz-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <iostream>
# include <netdb.h>
# include <cstring>
# include <cstdlib>
# include <cstdio>
# include <unistd.h>
# include <arpa/inet.h>
# include <sys/socket.h>
# include <sys/event.h>
# include <fcntl.h>
# include <vector>
# include <map>
# include "User.hpp"

# define MAX_EVENTS 64
# define CHUNK_SIZE 1025

/*FUNCTIONS IrcUtils*/
int		setNonBlocking(int serverSocket);
void	connection(std::string mensg, std::string pass, int *sockfd, std::map<int, User* > *clients);

/*FUNCTIONS InitSocket*/
void	init_server(int *serverSocket, int port);
void	init_program(int *serverSocket, std::string pass);

/*FUNCTIONS ComunicationUtils*/
void	logMessage(const std::string& message);
void	clientMessage(const std::string& message, int fd);
