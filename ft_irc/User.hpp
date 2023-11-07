#ifndef USER_HPP
#define USER_HPP

#include <iostream>
#include <string>

class User
{
    private:
        std::string _user;
        std::string _nick;
        std::string _pass;
        bool is_oper;
    public:
        ~User();
        User();

        setUser(std::string user);
        setNick(std::string nick);
        setPass(std::string pass);
        beOper();

        bool authenticate(std::stirng nick, std::string pass);
};

#endif