#pragma once
# include <iostream>
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
# include "response.hpp"

# define CHUNK_SIZE 1025

class Channel;
class User
{
    private:
        std::vector<Channel *> _channels;
        std::string _user;
        std::string _nick;
        std::string _realName;
        std::string _hostname;
        std::string _status;
        int         _port;
        int         _clientSocket;
        User();
        User(User const &src);
        User &operator=(User const &src);
    public:
        User(int serverSocket, int kq);
        ~User();

        int         getClientSocket();
        int         getPort();
        std::string getHostName();
        std::string getStatus();
        std::string getNick();

        void setStatus(std::string const status);
        void setUser(std::string const user);
        void setNick(std::string const nick);
        void setRealName(std::string const realName);

        std::string getPrefix()const;
        void reply(const std::string& reply);
        void clientMessage(const std::string& message)const;
        void join(Channel *channel);
        
};
