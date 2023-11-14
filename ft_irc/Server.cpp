#include "Server.hpp"

Server::Server()
{
}

Server::~Server()
{
}

void Server::Join(std::string canal)
{
    std::string msg;
    bool createChannel = true;
    for (channels_it it = this->_channels.begin(); it < this->_channels.end(); it++)
    {
        if (it->getName() == canal) //existe el canal, mensaje de que ha entrado
        {
            createChannel = false;
            msg = "";
            logMessage(msg);
        }
    }
    if (createChannel) //no existe el canal, crearlo
        createChannel(canal);
}

void Server::Part(std::string canal, User &user)
{
    std::string msg;
    bool doesExist = false;
    for (channels_it it = this->_channels.begin(); it < this->_channels.end(); it++)
    {
        if (it->getName() == canal) //existe el canal, eliminar al usuario
        {
            //mirar si el usuario forma parte del canal (con it??)
            doesExist = true;
            msg = "";
            logMessage(msg);
        }
    }
    if (!doesExist) //no existe
    {
        msg = "";
        logMessage(msg);
    }
}

Server::Msg()
{
}

Server::Quit()
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

Server::insertUser(User &user)
{
    this->_users.insert({user._port, user});
}

Server::createChannel(std::string name)
{
    Canal newChannel(name);
    this->_channels.push_back(newChannel);
}