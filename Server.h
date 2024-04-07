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
    std::atomic<bool> isLeader{false};
    std::vector<uint16_t> peerPorts; // Ports of other servers in the system

public:
    Server(const uint16_t& portNumber, const std::string& serverName);
    virtual ~Server();

    void acceptConnection();
    virtual void handleConnection(const int clientSock) = 0;

    void initiateElection();

    void announceLeadership();
    void receiveMessage(const std::string& message, int clientSock);
    void sendMessage(const std::string& message, uint16_t port);
    int createSocketAndConnect(uint16_t port);
};
#endif