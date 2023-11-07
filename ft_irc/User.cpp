#include "User.hpp"

User::~User()
{
}

User::User() : _user(NULL), _nick(NULL), _pass(NULL), is_oper(false)
{
}

User::setUser(std::string user) : _user(user)
{
}

User::setNick(std::string nick) : _nick(nick)
{
}

User::setPass(std::string pass) : _pass(pass)
{
}

User::beOper() : is_oper(true)
{
}

bool User::authenticate(std::string nick, std::string pass)
{
    if (this->_nick == nick && this->_pass == pass)
        return (true);
    return (false);
}