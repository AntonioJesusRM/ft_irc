#include "Server.hpp"

Server::Server(int port, std::string pass)
{
    this->_port = port;
    this->_pass = pass;
    struct sockaddr_in	serverAddr;
	
	this->_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (this->_serverSocket == -1) 
        throw std::runtime_error("Error: Create socket");

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(port);

    if (bind(this->_serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        close(this->_serverSocket);
        throw std::runtime_error("Error: Address already in use!");
    }

    fcntl(this->_serverSocket, F_SETFL, O_NONBLOCK);

    if (listen(this->_serverSocket, MAX_CONNECTIONS) == -1) {
        close(this->_serverSocket);
        throw std::runtime_error("Error: listening on a socket");
    }
}

Server::~Server()
{
    this->_users.clear();
    this->_channels.clear();
    close(this->_serverSocket);
}

static std::string read_message(int fd)
{
    std::string message;
    
    char buffer[100];
    bzero(buffer, 100);

    while (!strstr(buffer, "\n"))
    {
        bzero(buffer, 100);

        if ((recv(fd, buffer, 100, 0) < 0) and (errno != EWOULDBLOCK))
            throw std::runtime_error("Error while reading buffer from a client!");

        message.append(buffer);
    }

    return message;
}

void Server::start()
{
    int				kq;
	struct kevent	event;
	struct kevent	events[MAX_EVENTS];

	kq = kqueue();
    if (kq == -1) {
        close(this->_serverSocket);
        throw std::runtime_error("Error: create Kqueue");
    }
    EV_SET(&event, this->_serverSocket, EVFILT_READ, EV_ADD, 0, 0, NULL);
    kevent(kq, &event, 1, NULL, 0, NULL);
    logMessage("Server is listening...");
	while (true)
	{
        int numEvents = kevent(kq, NULL, 0, events, MAX_EVENTS, NULL);
		for (int i = 0; i < numEvents; ++i) {
            int sockfd = events[i].ident;
			if (sockfd == this->_serverSocket) {
                User *user = new User(this->_serverSocket, kq);
                this->_users[user->getClientSocket()] = user;
            }else {
                if (events[i].flags & EV_EOF)
                {
                    User* user = this->_users[sockfd];
                    std::string msg;
                    if (user->getNick().empty())
                        msg = this->_users[sockfd]->getHostName() + ":" + std::to_string(this->_users[sockfd]->getPort()) + " has disconnected!";
                    else
                        msg = user->getNick() + " has disconnected!";
                    logMessage (msg);
                    delete user;
                    this->_users.erase(sockfd);
                }
                else if (events[i].filter == EVFILT_READ)
                {
                    std::string meng = read_message(sockfd);
                    switchStatus(meng, sockfd);
                }
			}
        }
    }
}

Channel *Server::getChannel(std::string name)
{
    for (size_t i = 0; i < this->_channels.size(); i++)
    {
        if (name == this->_channels[i]->getName())
            return (this->_channels[i]);
    }
    return (NULL);
}

User *Server::getUser(std::string nick)
{
    for (std::map<int, User *>::iterator it = this->_users.begin(); it != this->_users.end(); ++it)
    {
        if (nick == it->second->getNick())
            return (it->second);
    }
    return (NULL);
}

void    Server::switchStatus(std::string const msg, int sockfd)
{
    if (this->_users[sockfd]->getStatus() == "CREATE")
    {
        if (this->clientConected(msg, sockfd) == 0)
            return ;
    }
    if (this->_users[sockfd]->getStatus() == "CONECTED")
    {
        this->clientRegistration(msg, sockfd);
        return ;
    }
    if (this->_users[sockfd]->getStatus() == "SIGNEDUP")
        this->switchCommand(msg, sockfd);
}

int Server::clientConected(std::string const msg, int sockfd)
{
    if (!getPassMsg(msg).empty() && this->_pass == getPassMsg(msg))
    {
            this->_users[sockfd]->setStatus("CONECTED");
			if (getUserMsg(msg).empty() || getRealNameMsg(msg).empty())
            {
                this->_users[sockfd]->clientMessage("You are connected, now set NICK and USER.");
				return 0;
            }
            this->_users[sockfd]->setUser(getUserMsg(msg));
            this->_users[sockfd]->setRealName(getRealNameMsg(msg));
            return 1;
    }
    else
    {
        User* user = this->_users[sockfd];
        std::string msgOut = this->_users[sockfd]->getHostName() + ":" + std::to_string(this->_users[sockfd]->getPort()) + " has disconnected because error password";
        logMessage (msgOut);
        this->_users[sockfd]->reply(ERR_PASSWDMISMATCH(this->_users[sockfd]->getNick()));
        delete user;
        this->_users.erase(sockfd);
        return 0;
    }
}

void Server::clientRegistration(std::string const msg, int sockfd)
{
    std::string nick = getNickMsg(msg);
	if (this->_users[sockfd]->getUser().empty())
	{
        if (!getUserMsg(msg).empty())
		    this->_users[sockfd]->setUser(getUserMsg(msg));
        if(!getRealNameMsg(msg).empty())
            this->_users[sockfd]->setRealName(getRealNameMsg(msg));
	}
    if (nick.empty())
    {
        return ;
    }
    for (std::map<int, User *>::iterator it = this->_users.begin(); it != this->_users.end(); ++it) {
        if (nick == it->second->getNick())
        {
            this->_users[sockfd]->reply(ERR_NICKNAMEINUSE((nick)));
            return ;
        }
    }
    this->_users[sockfd]->setStatus("SIGNEDUP");
    this->_users[sockfd]->setNick(nick);
    this->_users[sockfd]->reply(RPL_WELCOME(nick));
    std::string msgOut = this->_users[sockfd]->getHostName() + ":" + std::to_string(this->_users[sockfd]->getPort()) + " is now known as " + nick + ".";
    logMessage(msgOut);
}

void    Server::switchCommand(std::string const msg, int sockfd)
{
    std::string commands[] = {"JOIN", "PART", "PRIVMSG", "NICK", "USER", "KICK", "TOPIC", "INVITE", "MODE"};

    void (Server::*ExecCommand[9])(std::string msg, int sockfd) = {&Server::Join, &Server::Part, &Server::Msg, &Server::changeNick, &Server::changeUser, &Server::kick, &Server::Topic, &Server::Invite, &Server::Mode};
    
    std::string command = msg.substr(0, msg.find(" "));
    for (int i = 0; i < 9; i++)
    {
        if (command == commands[i])
        {
            (this->*ExecCommand[i])(msg, sockfd);
        }
    }
}

void Server::changeNick(std::string msg, int sockfd)
{
    std::string nick = getNickMsg(msg);
    if (nick .empty())
    {
        this->_users[sockfd]->reply(ERR_NEEDMOREPARAMS(this->_users[sockfd]->getNick(), "NICK"));
    }
    for (std::map<int, User *>::iterator it = this->_users.begin(); it != this->_users.end(); ++it) {
        if (nick == it->second->getNick())
        {
            this->_users[sockfd]->clientMessage(nick  + " :Nickname is already in use");
            return ;
        }
    }
    this->_users[sockfd]->setNick(nick);
    this->_users[sockfd]->reply(RPL_WELCOME(nick));
    std::string msgOut = this->_users[sockfd]->getHostName() + ":" + std::to_string(this->_users[sockfd]->getPort()) + " is now known as " + nick + ".";
    logMessage(msgOut);
}

void Server::changeUser(std::string msg, int sockfd)
{
    std::string user = getUserMsg(msg);
    if (user.empty())
    {
        this->_users[sockfd]->reply(ERR_NEEDMOREPARAMS(this->_users[sockfd]->getNick(), "USER"));
    }
    this->_users[sockfd]->setUser(user);
    this->_users[sockfd]->clientMessage(this->_users[sockfd]->getNick()  + " :change your user for " + user);
    std::string msgOut = this->_users[sockfd]->getNick() + " change user for " + user + ".";
    logMessage(msgOut);
}

void Server::Join(std::string const msg, int sockfd)
{
    std::vector<std::string> channelInfo = getInfoMsg(msg);
    
    std::string name = channelInfo[0];
    if (name.at(0) != '#')
        return;
    std::string pass = channelInfo.size() > 1 ? channelInfo[1] : "";
    Channel *channel = this->getChannel(name);
	if (!channel)
    {
		channel = new Channel(name, pass, this->_users[sockfd]);
        this->_channels.push_back(channel);
    }
    if (!this->_users[sockfd]->isInvite(channel->getName()))
    {
        if (channel->getPass() != pass)
        {
            this->_users[sockfd]->reply(ERR_BADCHANNELKEY(this->_users[sockfd]->getNick(), name));
            return;
        }
        if (channel->getI() % 2 != 0)
        {
            this->_users[sockfd]->reply(ERR_BADCHANNELINVITE(this->_users[sockfd]->getNick(), name));
            return;
        }
    }
    this->_users[sockfd]->join(channel);
}

void Server::Part(std::string const msg, int sockfd)
{
    std::vector<std::string> channelInfo = getInfoMsg(msg);
    std::string name = channelInfo[0];

    Channel *channel = this->getChannel(name);
	if (!channel || (channel->userChannel(this->_users[sockfd]) == -1))
    {
        this->_users[sockfd]->reply(ERR_NOSUCHCHANNEL(this->_users[sockfd]->getNick(), name));
        return;
    }
    channel->removeUser(channel->userChannel(this->_users[sockfd]), this->_users[sockfd]);
    this->_users[sockfd]->leaveChannel(channel);
}

void Server::Msg(std::string const msg, int sockfd)
{
    std::vector<std::string> msgInfo = getInfoMsg(msg);
    std::string dest = msgInfo[0];
    std::string message;
    std::vector<std::string>::iterator it = msgInfo.begin() + 1;
    std::vector<std::string>::iterator end = msgInfo.end();
    while (it != end)
    {
        message.append(*it + " ");
        it++;
    }
    if (message.at(0) == ':')
        message = message.substr(1);
    if (dest.at(0) == '#')
    {
        Channel *channel = this->getChannel(dest);
        if (!channel)
        {
            this->_users[sockfd]->reply(ERR_NOSUCHCHANNEL(this->_users[sockfd]->getNick(), dest));
			return;
        }
        if (channel->userChannel(this->_users[sockfd]) == -1)
        {
            this->_users[sockfd]->reply(ERR_CANNOTSENDTOCHAN(this->_users[sockfd]->getNick(), dest));
            return;
        }
        channel->broadcast(RPL_PRIVMSG(this->_users[sockfd]->getPrefix(), dest, message), this->_users[sockfd]);
        return;
    }
    else
    {
        User *user = this->getUser(dest);
        if (!user)
        {
            this->_users[sockfd]->reply(ERR_NOSUCHNICK(this->_users[sockfd]->getNick(), dest));
		    return;
        }
        user->clientMessage(RPL_PRIVMSG(this->_users[sockfd]->getPrefix(), dest, message));
    }
}

void Server::kick(std::string msg, int sockfd)
{
    std::vector<std::string> msgInfo = getInfoMsg(msg);
    Channel *channel = this->getChannel(msgInfo[0]);
    User *dest = this->getUser(msgInfo[1]);
    std::string reason = "No reason specified!";

    if (!channel)
    {
        this->_users[sockfd]->reply(ERR_NOSUCHCHANNEL(this->_users[sockfd]->getNick(), msgInfo[0]));
        return ;
    }
    if (!channel->isAdmin(this->_users[sockfd]))
    {
        this->_users[sockfd]->reply(ERR_CHANOPRIVSNEEDED(this->_users[sockfd]->getNick(), msgInfo[0]));
        return;
    }
    if (!dest)
    {
        this->_users[sockfd]->reply(ERR_NOSUCHNICK(this->_users[sockfd]->getNick(), msgInfo[1]));
        return ;
    }
    if (msgInfo.size() >= 3 && msgInfo[2].size() > 1)
    {
        reason = "";

        std::vector<std::string>::iterator it = msgInfo.begin() + 2;
        std::vector<std::string>::iterator end = msgInfo.end();

        while (it != end)
        {
			reason.append(*it + " ");
            it++;
        }
    }
    channel->kickUser(this->_users[sockfd], dest, reason);
}

void Server::Topic(std::string msg, int sockfd)
{
    std::vector<std::string> msgInfo = getInfoMsg(msg);
    Channel *channel = this->getChannel(msgInfo[0]);

    if (!channel)
    {
        this->_users[sockfd]->reply(ERR_NOSUCHCHANNEL(this->_users[sockfd]->getNick(), msgInfo[0]));
        return ;
    }
    if (channel->getT() % 2 == 0 && !channel->isAdmin(this->_users[sockfd]))
    {
        this->_users[sockfd]->reply(ERR_CHANOPRIVSNEEDED(this->_users[sockfd]->getNick(), msgInfo[0]));
        return;
    }
    std::string topic = channel->getTopic();
    if (msgInfo.size() >= 2)
    {
        topic = "";

        std::vector<std::string>::iterator it = msgInfo.begin() + 1;
        std::vector<std::string>::iterator end = msgInfo.end();

        while (it != end)
        {
			topic.append(*it + " ");
            it++;
        }
    }
    channel->printTopic(topic, this->_users[sockfd]);
}

void Server::Invite(std::string msg, int sockfd)
{
    std::vector<std::string> msgInfo = getInfoMsg(msg);
    Channel *channel = this->getChannel(msgInfo[1]);
    User *dest = this->getUser(msgInfo[0]);

    if (!channel)
    {
        this->_users[sockfd]->reply(ERR_NOSUCHCHANNEL(this->_users[sockfd]->getNick(), msgInfo[0]));
        return ;
    }
    if (!channel->isAdmin(this->_users[sockfd]))
    {
        this->_users[sockfd]->reply(ERR_CHANOPRIVSNEEDED(this->_users[sockfd]->getNick(), msgInfo[0]));
        return;
    }
    if (!dest)
    {
        this->_users[sockfd]->reply(ERR_NOSUCHNICK(this->_users[sockfd]->getNick(), msgInfo[1]));
        return ;
    }
    channel->sendInvite(dest, this->_users[sockfd]);
}

void Server::Mode(std::string msg, int sockfd)
{
    std::vector<std::string> msgInfo = getInfoMsg(msg);

    if (msgInfo.size() < 2)
    {
        this->_users[sockfd]->reply(ERR_NEEDMOREPARAMS(this->_users[sockfd]->getNick(), "MODE"));
        return;
    }
    Channel *channel = this->getChannel(msgInfo[0]);
    if (!channel)
    {
        this->_users[sockfd]->reply(ERR_NOSUCHCHANNEL(this->_users[sockfd]->getNick(), msgInfo[0]));
        return ;
    }
    if (!channel->isAdmin(this->_users[sockfd]))
    {
        this->_users[sockfd]->reply(ERR_CHANOPRIVSNEEDED(this->_users[sockfd]->getNick(), msgInfo[0]));
        return;
    }
    std::string commands[] = {"i", "t", "k", "l"};
    void (Channel::*ExecCommand[4])(std::vector<std::string> msgInfo, User *user) = {&Channel::ChangeI, &Channel::ChangeT, &Channel::ChangeK, &Channel::ChangeL};
    std::string command = msgInfo[1].substr(1);
    for (int i = 0; i < 4; i++)
    {
        if (command == commands[i])
        {
            (channel->*ExecCommand[i])(msgInfo, this->_users[sockfd]);
        }
    }
    if (command == "o")
    {
        if (msgInfo.size() < 3)
        {
            this->_users[sockfd]->reply(ERR_NEEDMOREPARAMS(this->_users[sockfd]->getNick(), "MODE"));
            return;
        }
        User *dest = this->getUser(msgInfo[2]);
        if (!dest)
        {
            this->_users[sockfd]->reply(ERR_NOSUCHNICK(this->_users[sockfd]->getNick(), msgInfo[1]));
            return ;
        }
        channel->ChangeO(msgInfo, this->_users[sockfd], dest);
    }
}
