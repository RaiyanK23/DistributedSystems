#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>
#include<cstring>
#include<thread>

#define PORT 8080

class Proxy
{
private:
    int serverSock, clientSock;
    struct sockaddr_in serverInfo, clientInfo;
    socklen_t clientSize;

public:
    Proxy();
    ~Proxy();

    void acceptConnection();
    void handleConnection(int clientSock);
};