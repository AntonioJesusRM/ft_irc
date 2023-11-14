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
            this->_users[sockfd]->setUser(getUserMsg(msg));
            this->_users[sockfd]->setRealName(getRealNameMsg(msg));
            return 1;
    }
    else
    {
        User* user = this->_users[sockfd];
        std::string msgOut = this->_users[sockfd]->getHostName() + ":" + std::to_string(this->_users[sockfd]->getPort()) + " has disconnected because error password";
        logMessage (msgOut);
        this->_users[sockfd]->badPassword();
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
            this->_users[sockfd]->badNickNameTry(nick);
            return ;
        }
    }
    this->_users[sockfd]->setStatus("SIGNEDUP");
    this->_users[sockfd]->setNick(nick);
    this->_users[sockfd]->welcome();
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
    /*if (command == "NICK")
        this->changeNick(msg, sockfd);
    else
    {
        std::cout << this->_users[sockfd]->getNick() << " esta " << this->_users[sockfd]->getStatus() << std::endl;
        std::cout << msg << std::endl;
    }*/
}

void Server::changeNick(std::string msg, int sockfd)
{
    std::string nick = msg.substr(msg.find(" ") + 1);
    nick = nick.substr(0, nick.find('\r'));
    for (std::map<int, User *>::iterator it = this->_users.begin(); it != this->_users.end(); ++it) {
        if (nick == it->second->getNick())
        {
            this->_users[sockfd]->badNickName(nick);
            return ;
        }
    }
    this->_users[sockfd]->setNick(nick);
    this->_users[sockfd]->welcome();
    std::string msgOut = this->_users[sockfd]->getHostName() + ":" + std::to_string(this->_users[sockfd]->getPort()) + " is now known as " + nick + ".";
    logMessage(msgOut);
}

void Server::Join(std::string const msg, int sockfd)
{
    std::string channel = msg.substr(msg.find(" ") + 1);
    channel = channel.substr(0, channel.find('\r'));

    std::string users = "";

    for (size_t i = 0; i < _channels.size(); i++)
    {
        if (channel == _channels[i]->getName())
        {
            users.append(_channels[i]->getUsers(channel));
        }
    }

    this->_users[sockfd]->Join(channel, users);
    (void)msg;
    (void)sockfd;
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