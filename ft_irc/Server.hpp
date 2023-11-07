#ifndef SERVER_HPP
#define SERVER_HPP

#include "User.hpp"
#include "Canal.hpp"
#include <iostream>

class Server
{
    private:
        std::vector<Canal> canals;
        std::map<int, User> users;
        int serverSocket;
    public:
        Server();
        ~Server();
        
        std::string IsACommand(std::string meng);
        Join(std::string canal);
        Part(std::string canal);
        Msg();

        //privilegios de oper
        Kick();
        Mode();
        Invite();
        Topic();
        
        //insertar usuarios
        //crear canal
}

#endif