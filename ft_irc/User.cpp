#include "User.hpp"

User::User(){}

User::User(int port, std::string const &hostname) : _user(""), _nick(""), _realName(""), _hostname(hostname), _port(port)
{
}

User::~User()
{
}


void User::setUser(std::string user){this->_user = user;}

void User::setNick(std::string nick){this->_nick = nick;}

void User::setRealName(std::string realName){this->_realName = realName;}

/*void User::setPass(std::string pass){this->_pass = pass;}

std::string User::getUser(){return (this->_user);}*/

std::string User::getNick(){return (this->_nick);}

//std::string User::getPass(){return (this->_pass);}

std::string User::getHostname(){return (this->_hostname);}

int User::getPort(){return (this->_port);}

/*void User::beOper(){this->is_oper = true;}

bool User::authenticate(std::string nick, std::string pass)
{
    if (this->_nick == nick && this->_pass == pass)
        return (true);
    return (false);
}*/
