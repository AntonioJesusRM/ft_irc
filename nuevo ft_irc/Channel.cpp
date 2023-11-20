#include "Channel.hpp"

Channel::Channel(std::string name, std::string pass, User* admin) : _name(name), _pass(pass), _admin(admin){}
Channel::~Channel(){}

std::string Channel::getName(){return (this->_name);}
std::string Channel::getPass(){return (this->_pass);}
std::string Channel::getUsers()
{
    std::string users = "";

    for (size_t i = 0; i < _users.size(); i++)
    {
        users.append("@" + this->_users[i]->getNick() + " ");
    }
    return (users);
}

void    Channel::addUser(User* user)
{
    this->_users.push_back(user);
}

void    Channel::broadcast(std::string msg)
{
    std::cout << msg << std::endl;
    for(size_t i = 0; i < this->_users.size(); i++)
    {
        this->_users[i]->clientMessage(msg);
    }
}

void    Channel::broadcast(std::string msg, User* exclude)
{
    for(size_t i = 0; i < this->_users.size(); i++)
    {
        if (exclude == this->_users[i])
            continue;
        this->_users[i]->clientMessage(msg);
    }
}

int Channel::userChannel(User *user)
{
    for (size_t i = 0; i < this->_users.size(); i++)
    {
        if (user->getNick() == this->_users[i]->getNick())
            return (i);
    }
    return (-1);
}

void Channel::removeUser(int pos, User *user)
{
    this->_users.erase(this->_users.begin() + pos);
    if (user == this->_admin)
    {
        if (this->_users.size() > 0)
        {
            this->_admin = *(this->_users.begin());
            std::string message = this->_admin->getNick() + " is now the admin of the channel " + this->_name;
            logMessage(message);
        }
        else
            delete this;
    }
}

bool Channel::isAdmin(User *user)
{
    if (this->_admin == user)
        return true;
    return false;
}

void    Channel::kickUser(User *admin, User *dest, std::string reason)
{
    this->broadcast(RPL_KICK(admin->getPrefix(), _name, dest->getNick(), reason));
    this->removeUser(this->userChannel(dest), dest);

    std::string message = admin->getNick() + " kicked " + dest->getNick() + " from channel " + _name;
    logMessage(message);
}
