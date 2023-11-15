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
# include "Channel.hpp"
# include "User.hpp"
# include "response.hpp"

# define MAX_CONNECTIONS 999
# define MAX_EVENTS 1024

class Server
{
    private:
        int                     _serverSocket;
        int                     _port;
        std::string             _pass;
        std::vector<Channel *>   _channels;
        std::map<int, User *>   _users;
        
        Server();
        Server(Server const &src);
        Server &operator=(Server const &src);
    public:
        Server(int  port, std::string pass);
        ~Server();

        void start();
        Channel *getChannel(std::string name);
        void switchStatus(std::string const msg, int sockfd);
        
        int clientConected(std::string const msg, int sockfd);
        void clientRegistration(std::string const msg, int sockfd);
        void switchCommand(std::string const msg, int sockfd);
        
        void changeNick(std::string msg, int sockfd);
        void changeUser(std::string msg, int sockfd);

        void Join(std::string msg, int sockfd);
        void Part(std::string msg, int sockfd);
        void Msg(std::string msg, int sockfd);
};
