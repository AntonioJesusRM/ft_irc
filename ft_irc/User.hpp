#pragma once
#include <iostream>

class User
{
    private:
        std::string _user;
        std::string _nick;
        std::string _realName;
        //std::string _pass;
        std::string _hostname;
        int         _port;
        //bool        is_oper;
        User();
    public:
        User(int port, std::string const &hostname);
        ~User();

        void setUser(std::string user);
        void setNick(std::string nick);
        void setRealName(std::string realName);
        //void setPass(std::string pass);

        //std::string getUser();
        std::string getNick();
        //std::string getPass();
        std::string getHostname();
        int         getPort();

        /*void beOper();

        bool authenticate(std::string nick, std::string pass);*/
};
