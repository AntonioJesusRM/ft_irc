#ifndef CANAL_HPP
#define CANAL_HPP

class Canal
{
    private:
        std::string _name;
        std::map<int, User> users;
    public:
        Canal(std::string name);
        ~Canal();

        std::string getName() const;
        void newUser(User &user); //añadir un usuario al canal
}

#endif