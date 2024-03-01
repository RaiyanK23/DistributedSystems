#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>
#include<cstring>
#include<thread>

#define NYSEPORT 8010

class NYSEServer
{
private:
    int serverSock, clientSock;
    struct sockaddr_in serverInfo, clientInfo;
    socklen_t clientSize;

public:
    NYSEServer();
    ~NYSEServer();

    void acceptConnection();
    void handleConnection(int clientSock);
};