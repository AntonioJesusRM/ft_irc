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
                char buffer[1024];
				memset(buffer, '\0', sizeof(buffer));
                ssize_t bytesRead = recv(sockfd, buffer, sizeof(buffer), 0);
                if (bytesRead <= 0) {
                    User* user = this->_users[sockfd];
                    std::string msg = this->_users[sockfd]->getHostName() + ":" + std::to_string(this->_users[sockfd]->getPort()) + " has disconnected!";
                    logMessage (msg);
                    delete user;
                    this->_users.erase(sockfd);
                } else {
					std::string meng = buffer;
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
    {
        std::cout << "#####ENTRADA: " << msg << std::endl;
        this->switchCommand(msg, sockfd);
    }
}

int Server::clientConected(std::string const msg, int sockfd)
{
    if (!getPassMsg(msg).empty() && this->_pass == getPassMsg(msg))
    {
            this->_users[sockfd]->setStatus("CONECTED");
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
    std::string commands[] = {"JOIN", "PART", "MSG", "NICK"};

    void (Server::*ExecCommand[4])(std::string msg, int sockfd) = {&Server::Join, &Server::Part, &Server::Msg, &Server::changeNick};
    
    std::string command = msg.substr(0, msg.find(" "));
    for (int i = 0; i < 4; i++)
    {
        if (command == commands[i])
        {
            (this->*ExecCommand[i])(msg, sockfd);
        }
    }
}

void Server::changeNick(std::string msg, int sockfd)
{
    std::string nick = msg.substr(msg.find(" ") + 1);
    nick = nick.substr(0, nick.find('\r'));
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

void Server::Join(std::string const msg, int sockfd)
{
    std::vector<std::string> channelInfo = getChannelMsg(msg);
    
    std::string name = channelInfo[0];
    std::string pass = channelInfo.size() > 1 ? channelInfo[1] : "";
    Channel *channel = this->getChannel(name);
	if (!channel)
    {
		channel = new Channel(name, pass, this->_users[sockfd]);
        this->_channels.push_back(channel);
    }
    if (channel->getPass() != pass)
    {
        std::cout << "error" << std::endl;
        this->_users[sockfd]->reply(ERR_BADCHANNELKEY(this->_users[sockfd]->getNick(), name));
		return;
	}
    this->_users[sockfd]->join(channel);
}

void Server::Part(std::string const msg, int sockfd)
{
    (void)msg;
    (void)sockfd;
}

void Server::Msg(std::string const msg, int sockfd)
{
    (void)msg;
    (void)sockfd;
}