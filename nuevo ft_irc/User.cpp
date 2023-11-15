#include "User.hpp"

User::User(int serverSocket, int kq)
{
    struct kevent       event;
    struct sockaddr_in6 clientAddr;

    socklen_t clientAddrLen = sizeof(clientAddr);
    this->_clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
    char hostname[CHUNK_SIZE];
    int res = getnameinfo((struct sockaddr *) &clientAddr, sizeof(clientAddr), hostname, CHUNK_SIZE, NULL, 0, NI_NUMERICSERV);
    if (res != 0)
        std::cerr << "Error while getting a hostname on a new client!" << std::endl;
    fcntl(this->_clientSocket, F_SETFL, O_NONBLOCK);
    EV_SET(&event, this->_clientSocket, EVFILT_READ, EV_ADD, 0, 0, NULL);
    kevent(kq, &event, 1, NULL, 0, NULL);
    this->_port = ntohs(clientAddr.sin6_port);
    this->_hostname = hostname;
    logMessage(this->_hostname + ":" + std::to_string(this->_port) + " has connected.");
    this->_status = "CREATE";
    this->_nick = "";
    this->_user = "";
    this->_realName = "";
}

User::~User()
{
    close(this->_clientSocket);
}

int User::getClientSocket(){return (this->_clientSocket);}
int User::getPort(){return (this->_port);}
std::string User::getHostName(){return (this->_hostname);}
std::string User::getStatus(){return (this->_status);}
std::string User::getNick(){return (this->_nick);}

void User::setStatus(std::string const status){this->_status = status;}
void User::setUser(std::string const user){this->_user = user;}
void User::setRealName(std::string const realName){this->_realName = realName;}
void User::setNick(std::string const nick){this->_nick = nick;}

std::string User::getPrefix() const 
{
    std::string username = this->_user.empty() ? "" : "!" + this->_user;
    std::string hostname = this->_hostname.empty() ? "" : "@" + this->_hostname;

    return this->_nick + username + hostname;
}

void User::clientMessage(const std::string& message)const
{
    std::string buffer = message + "\r\n";
    if (send(this->_clientSocket, buffer.c_str(), buffer.length(), 0) < 0)
        std::cerr << "Error send message." << std::endl;
}

void User::reply(const std::string& reply)
{
    this->clientMessage(":" + this->getPrefix() + " " + reply);
}

void User::join(Channel *channel)
{
    channel->addUser(this);
    this->_channels.push_back(channel);
    std::string users = channel->getUsers();

    this->reply(RPL_NAMREPLY(this->_nick, channel->getName(), users));
    this->reply(RPL_ENDOFNAMES(this->_nick, channel->getName()));
    channel->broadcast(RPL_JOIN(getPrefix(), channel->getName()));

    std::string message = this->_nick + " has joined to the channel " + channel->getName();
    logMessage(message);
}

void User::leaveChannel(Channel *channel)
{
    std::string name = channel->getName();

    for (size_t i = 0; i < this->_channels.size(); i++)
        if (name == this->_channels[i]->getName())
            this->_channels.erase(this->_channels.begin() + i);
    channel->broadcast(RPL_PART(getPrefix(), channel->getName()));
    std::string message = _nick + " has left the channel " + name;
    logMessage(message);
}
