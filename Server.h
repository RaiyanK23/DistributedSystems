#ifndef SERVER_H
#define SERVER_H
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdint>
#include <arpa/inet.h>
#include <iostream>
#include<cstring>
#include<thread>
#include<string>

class Server
{
protected:
    int serverSock, clientSock;
    struct sockaddr_in serverInfo, clientInfo;
    socklen_t clientSize;
    const std::string sName;
    const uint16_t port;

public:
    Server(const uint16_t& portNumber, const std::string& serverName);
    virtual ~Server();

    void acceptConnection();
    virtual void handleConnection(const int clientSock) = 0;
};
#endif