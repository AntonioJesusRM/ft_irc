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
# include "Utils.hpp"
# include "Chanel.hpp"
# include "User.hpp"

# define MAX_CONNECTIONS 999
# define MAX_EVENTS 1024

class Server
{
    private:
        int                     _serverSocket;
        int                     _port;
        std::string             _pass;
        //std::vector<Chanel *>   _chanels;
        std::map<int, User *>   _users;
        
        Server();
        Server(Server const &src);
        Server &operator=(Server const &src);
    public:
        Server(int  port, std::string pass);
        ~Server();

        void start();
        void swithcMessage(std::string const meng, int sockfd);
};
