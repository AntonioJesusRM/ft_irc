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
        std::string         _topic;
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
        std::string getTopic();

        void    addUser(User* user);
        void    broadcast(std::string msg);
        void    broadcast(std::string msg, User *exclude);
        int     userChannel(User *user);
        void    removeUser(int pos, User *user);
        bool    isAdmin(User *user);
        void    kickUser(User *admin, User *dest, std::string reason);
        void    printTopic(std::string topic);
        void    sendInvite(User *dest);
};
