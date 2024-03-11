#include "Server.h"

Server::Server(const uint16_t& portNumber, const std::string& serverName) : port(portNumber), sName(serverName)
{
    if((serverSock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        std::cerr << "Proxy server socket creation has failed!\n";
        exit(1);
    }

    memset(&serverInfo, 0, sizeof(serverInfo));
    serverInfo.sin_family = AF_INET;
    serverInfo.sin_port = htons(portNumber);
    serverInfo.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(serverSock, (sockaddr*)&serverInfo, sizeof(serverInfo)) == -1)
    {
        std::cerr << "Bind has failed for proxy server setup!" << '\n';
        close(serverSock);
        exit(1);
    }
    std::cout << sName +" now running on port " << portNumber << '\n';

    if(listen(serverSock, 5) == -1)
    {
        std::cerr << "Listen for proxy server setup has failed!" << '\n';
        close(serverSock);
        exit(1);
    }

    std::cout << sName+ " is now listening!\n"; 
}

Server::~Server()
{
    close(serverSock);
}

void Server::acceptConnection()
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

        std::cout << sName+ " accepted a client connection!\n";

        std::thread(&Server::handleConnection, this, clientSock).detach();
    }
}
