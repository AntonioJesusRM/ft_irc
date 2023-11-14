#include "Canal.hpp"

Canal::Canal(std::string name) : _name(name)
{
}

Canal::~Canal()
{
}

std::string Canal::getName() const
{
    return (this->_name);
}

void Canal::newUser(User &user)
{
    this->_users.insert({user._port, user});
}