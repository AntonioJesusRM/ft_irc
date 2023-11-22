#pragma once
# include <iostream>
# include <map>
# include "User.hpp"

class User;
class Channel
{
    private:
        std::string             _name;
        std::string             _pass;
        std::string             _topic;
        std::vector<User *>     _users;
        std::vector<User *>     _admin;
        int                     _i;
        int                     _t;
        int                     _o;
        int                     _k;
        int                     _l;
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
        int getI();

        void    addUser(User* user);
        void    broadcast(std::string msg);
        void    broadcast(std::string msg, User *exclude);
        int     userChannel(User *user);
        void    removeUser(int pos, User *user);
        bool    isAdmin(User *user);
        void    kickUser(User *admin, User *dest, std::string reason);
        void    printTopic(std::string topic, User *user);
        void    sendInvite(User *dest, User *user);

        void ChangeI(std::vector<std::string> msgInfo, User *user);
        void ChangeT(std::vector<std::string> msgInfo, User *user);
        void ChangeO(std::vector<std::string> msgInfo, User *user);
        void ChangeK(std::vector<std::string> msgInfo, User *user);
        void ChangeL(std::vector<std::string> msgInfo, User *user);
};
