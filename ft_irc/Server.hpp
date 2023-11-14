#ifndef SERVER_HPP
#define SERVER_HPP

#include "User.hpp"
#include "Canal.hpp"
#include <iostream>

class Server
{
    typedef std::vector<Canal>::iterator channels_it;
    private:
        std::vector<Canal> _channels;
        std::map<int, User> _users;
        int _serverSocket;
    public:
        Server();
        ~Server();
        
        std::string IsACommand(std::string meng);
        void Join(std::string canal);
        void Part(std::string canal, User &user);
        Msg();

        Quit();

        //privilegios de oper
        Kick();
        Mode();
        Invite();
        Topic();
        
        insertUser(User &user);//insertar usuarios
        createChannel(std::string name);//crear canal
}

#endif