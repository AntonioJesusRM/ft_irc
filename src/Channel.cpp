#include "../includes/Channel.hpp"

Channel::Channel(std::string name, std::string pass, User* admin) : _name(name), _pass(pass), _topic("")
{
    this->_admin.push_back(admin);
    this->_i = false;
    this->_t = false;
    this->_k = false;
    this->_l = 0;
}

Channel::~Channel(){}

std::string Channel::getName(){return (this->_name);}
std::string Channel::getPass(){return (this->_pass);}
std::string Channel::getTopic(){return (this->_topic);}
int Channel::getSizeUsers(){return (this->_users.size());}
bool Channel::getI(){return (this->_i);}
bool Channel::getT(){return (this->_t);}
int Channel::getL(){return (this->_l);}
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

void Channel::removeUser(User *user)
{
    this->_users.erase(this->_users.begin() + this->userChannel(user));
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
    this->removeUser(dest);

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
    bool active = false;
	if (msgInfo[1].at(0) == '+')
		active = true;
	if (active != this->getI())
	{
    	this->broadcast(RPL_MODE(user->getPrefix(), msgInfo[0], (active ? "+i" : "-i"), ""));
		this->_i = active;
	}
}

void Channel::ChangeT(std::vector<std::string> msgInfo, User *user)
{
	bool active = false;
	if (msgInfo[1].at(0) == '+')
		active = true;
	if (active != this->getT())
	{
    	this->broadcast(RPL_MODE(user->getPrefix(), msgInfo[0], (active ? "+t" : "-t"), ""));
		this->_t = active;
	}
}

void Channel::ChangeO(std::vector<std::string> msgInfo, User *user, User *dest)
{
	if (msgInfo[1].at(0) == '+')
	{
		this->_admin.push_back(dest);
		this->broadcast(RPL_MODE_O(user->getPrefix(), msgInfo[0], dest->getNick()));
	}
	else
	{
		if (!this->isAdmin(dest))
		{
			dest->reply(ERR_CHANOPRIVSNEEDED(dest->getNick(), msgInfo[0]));
			return ;
		}
		for (size_t i = 0; i < this->_admin.size(); i++)
		{
			if (dest->getNick() == this->_admin[i]->getNick())
            {
				this->_admin.erase(this->_admin.begin() + i);
                this->broadcast(RPL_MODE_MINUS_O(user->getPrefix(), msgInfo[0], dest->getNick()));
            }
		}
	}
}

void Channel::ChangeK(std::vector<std::string> msgInfo, User *user)
{
    if (msgInfo[1].at(0) == '+')
	{
        this->broadcast(RPL_MODE_K(user->getPrefix(), msgInfo[0], msgInfo[2]));
        this->_pass = msgInfo[2];
    }
	else
	{
		this->broadcast(RPL_MODE_MINUS_K(user->getPrefix(), msgInfo[0]));
        this->_pass = "";
	}
}

void Channel::ChangeL(std::vector<std::string> msgInfo, User *user)
{
    int number;
    const char *pass = msgInfo[2].c_str();
    if (msgInfo[1].at(0) == '+')
	{
        for (const char* ptr = pass; *ptr != '\0'; ++ptr) {
            if (!isdigit(*ptr)) {
                user->reply(ERR_NEEDMOREPARAMS(user->getNick(), "MODE"));
                return;
            }
        }
	    number = std::atoi(pass);
        if (number <= 0)
        {
            user->reply(ERR_NEEDMOREPARAMS(user->getNick(), "MODE"));
            return;
        }
        this->broadcast(RPL_MODE_L(user->getPrefix(), msgInfo[0], msgInfo[2]));
        this->_l = number;
    }
	else
	{
		this->broadcast(RPL_MODE_MINUS_L(user->getPrefix(), msgInfo[0]));
        this->_l = 0;
	}
}
