#pragma once
# include <iostream>
# include <map>
# include "User.hpp"

class User;
class Channel
{
    private:
        std::string         _name;
        std::string         _pass;
        std::vector<User *>   _users;
        User *              _admin;
        Channel();
        Channel(Channel const &src);
        Channel &operator=(Channel const &src);
    public:
        Channel(std::string name, std::string pass, User* admin);
        ~Channel();

        std::string getName();
        std::string getPass();
        std::string getUsers();

        void    addUser(User* user);
        void    broadcast(std::string msg);
        int     userChannel(User *user);
        void    removeUser(int pos, User *user);
};
