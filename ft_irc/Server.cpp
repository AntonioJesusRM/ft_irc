#include "Server.hpp"

Server::Server()
{
}

Server::~Server()
{
}

Server::Join(std::string canal)
{
    for (channels_it it = this->_channels.begin(); it < this->_channels.end(); it++)
    {
        if (it->getName() == canal)
            //existe el canal, mensaje de que ha entrado
    }
    //no existe el canal, crearlo
}

Server::Part(std::string canal)
{
}

Server::Msg()
{
}

std::string Server::IsACommand(std::string meng)
{
    std::string commands[] = {"JOIN", "PART", "MSG"};

    void (Server::*ExecCommand[3])(std::string meng) = {&Server::Join, &Server::Part, &Server::Msg};
    
    for (int i = 0; i < 3; i++)
    {
        if (meng.find(commands[i] != std::string::npos))
        {
            return ("Executed");
        }
    }
    return ("Not found");
}

Server::insertUser()
{
}

Server::createChannel()
{
}