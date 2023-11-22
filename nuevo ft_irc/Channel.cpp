#include "Channel.hpp"

Channel::Channel(std::string name, std::string pass, User* admin) : _name(name), _pass(pass), _topic("")
{
    this->_admin.push_back(admin);
    this->_i = 0;
    this->_t = 0;
    this->_o = 0;
    this->_k = 0;
    this->_l = 0;
}

Channel::~Channel(){}

std::string Channel::getName(){return (this->_name);}
std::string Channel::getPass(){return (this->_pass);}
std::string Channel::getTopic(){return (this->_topic);}
int Channel::getI(){return (this->_i);}
std::string Channel::getUsers()
{
    std::string users = "";

    for (size_t i = 0; i < _users.size(); i++)
    {
        users.append(this->_users[i]->getNick() + " ");
    }
    return (users);
}

void    Channel::addUser(User* user)
{
    this->_users.push_back(user);
}

void    Channel::broadcast(std::string msg)
{
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
    if (this->isAdmin(user) && this->_admin.size() == 0)
    {
        if (this->_users.size() > 0)
        {
            this->_admin.push_back(*(this->_users.begin()));
            std::string message = this->_admin[0]->getNick() + " is now the admin of the channel " + this->_name;
            logMessage(message);
        }
        else
            delete this;
    }
}

bool Channel::isAdmin(User *user)
{
    for (size_t i = 0; i < this->_admin.size(); i++)
    {
        if (this->_admin[i] == user)
            return true;
    }
    return false;
}

void    Channel::kickUser(User *admin, User *dest, std::string reason)
{
    this->broadcast(RPL_KICK(admin->getPrefix(), _name, dest->getNick(), reason));
    this->removeUser(this->userChannel(dest), dest);

    std::string message = admin->getNick() + " kicked " + dest->getNick() + " from channel " + _name;
    logMessage(message);
}

void    Channel::printTopic(std::string topic, User *user)
{
    if (topic.empty())
        topic = "not defined";
    else if (topic.at(0) == ':')
        topic = topic.substr(1);
    this->_topic = topic;
    this->broadcast(RPL_TOPIC(user->getPrefix(), this->_name, topic));
}

void    Channel::sendInvite(User *dest, User *user)
{
    dest->setChannelInvite(this->_name);
    dest->clientMessage(RPL_INVITE(user->getPrefix(), dest->getNick(), this->_name));
}

void Channel::ChangeI(std::vector<std::string> msgInfo, User *user)
{
    this->_i++;
    bool active = true;
    if (this->_i % 2 == 0)
        active = false;
    this->broadcast(RPL_MODE(user->getPrefix(), msgInfo[0], (active ? "+i" : "-i"), ""));
}

void Channel::ChangeT(std::vector<std::string> msgInfo, User *user)
{
    this->_t++;
    std::cout << this->_t << std::endl;
    (void)user;
    (void)msgInfo;
}

void Channel::ChangeO(std::vector<std::string> msgInfo, User *user)
{
    this->_o++;
    std::cout << this->_o << std::endl;
    (void)user;
    (void)msgInfo;
}

void Channel::ChangeK(std::vector<std::string> msgInfo, User *user)
{
    this->_k++;
    std::cout << this->_k << std::endl;
    (void)user;
    (void)msgInfo;
}

void Channel::ChangeL(std::vector<std::string> msgInfo, User *user)
{
    this->_l++;
    std::cout << this->_l << std::endl;
    (void)user;
    (void)msgInfo;
}
