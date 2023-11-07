#include "Server.hpp"

Server::Server()
{
}

Server::~Server()
{
}

Server::Join(std::string canal)
{
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