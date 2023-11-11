#pragma once
# include <iostream>
# include <map>
# include "User.hpp"
class Chanel
{
    private:
        std::string _name;
        std::map<std::string, User> users;
    public:
        Chanel();
        ~Chanel();
};
