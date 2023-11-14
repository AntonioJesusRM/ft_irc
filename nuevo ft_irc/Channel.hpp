#pragma once
# include <iostream>
# include <map>
# include "User.hpp"
class Channel
{
    private:
        std::string _name;
        User * _users;
    public:
        Channel(std::string name);
        ~Channel();

        std::string getName() const;
        std::string getUsers() const;
};
