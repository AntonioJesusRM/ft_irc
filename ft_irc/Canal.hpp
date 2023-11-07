#ifndef CANAL_HPP
#define CANAL_HPP

class Canal
{
    private:
        std::string _name;
        std::map<std::string, User> users;
    public:
        Canal();
        ~Canal();
}

#endif