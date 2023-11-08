/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ircserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruiz-mo <aruiz-mo@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 09:19:29 by aruiz-mo          #+#    #+#             */
/*   Updated: 2023/11/08 15:03:33 by aruiz-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "iostream"
# include "cstring"
# include "cstdlib"
# include "cstdio"
# include "unistd.h"
# include "arpa/inet.h"
# include "sys/socket.h"
# include "sys/event.h"
# include "fcntl.h"
# include "vector"
# include "map"

# define MAX_EVENTS 64
# define CHUNK_SIZE 1024

/*FUNCTIONS IrcUtils*/
int		setNonBlocking(int serverSocket);
void	connection(std::string mensg, std::string pass, int *sockfd, std::map<int, std::string> *clients);

/*FUNCTIONS InitSocket*/
void	init_server(int *serverSocket, int port);
void	init_program(int *serverSocket, std::string pass);

/*FUNCTIONS ComunicationUtils*/
void	log(const std::string& message);