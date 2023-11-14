#include "Channel.hpp"

Channel::Channel(std::string name) : _name(name)
{
}

Channel::~Channel()
{
}

std::string Channel::getName() const
{
    return (this->_name);
}

std::string Channel::getUsers() const
{
    std::string users = "";

    for (size_t i = 0; i < _users.size(); i++)
    {
        users.append(this->_users[i].getNick() + " ");
    }
    return (users);
}