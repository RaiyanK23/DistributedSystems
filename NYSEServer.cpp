#include "NYSEServer.h"

NYSEServer::NYSEServer()
{
    if((serverSock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        std::cerr << "Proxy server socket creation has failed!\n";
        exit(1);
    }

    memset(&serverInfo, 0, sizeof(serverInfo));
    serverInfo.sin_family = AF_INET;
    serverInfo.sin_port = htons(NYSEPORT);
    serverInfo.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(serverSock, (sockaddr*)&serverInfo, sizeof(serverInfo)) == -1)
    {
        std::cerr << "Bind has failed for proxy server setup!" << '\n';
        close(serverSock);
        exit(1);
    }
    std::cout << "NYSEServer now running on port " << NYSEPORT << '\n';

    if(listen(serverSock, 5) == -1)
    {
        std::cerr << "Listen for proxy server setup has failed!" << '\n';
        close(serverSock);
        exit(1);
    }

    std::cout << "NYSEServer is now listening!\n";   
}

NYSEServer::~NYSEServer()
{
    close(serverSock);
}

void NYSEServer::acceptConnection()
{
    while(true)
    {
        clientSize = sizeof(clientInfo);
        if((clientSock = accept(serverSock, (sockaddr*)&clientInfo, &clientSize)) == -1)
        {
            std::cerr << "Server accept has failed!\n";
            close(serverSock);
            exit(1);
        }

        std::cout << "NYSEServer accepted a client connection!\n";

        std::thread(&NYSEServer::handleConnection, this, clientSock).detach();
    }
}

void NYSEServer::handleConnection(int clientSock)
{
    close(clientSock);
}