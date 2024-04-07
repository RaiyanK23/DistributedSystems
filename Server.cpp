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


void Server::handleConnection(int clientSock) {
    char buffer[1024] = {0};
    ssize_t bytesReceived = read(clientSock, buffer, 1024);
    if (bytesReceived > 0) {
        std::string message(buffer);
        receiveMessage(message, clientSock);
    }
    close(clientSock);
}

void Server::initiateElection() {
    std::cout << sName << " initiating an election." << std::endl;
    for (auto& peerPort : peerPorts) {
        sendMessage("ELECTION", peerPort);
    }
    // This server assumes leadership until someone with a higher port responds
    isLeader = true;
    for (auto& peerPort : peerPorts) {
        if (peerPort > port) {
            isLeader = false;
            break;
        }
    }
    if (isLeader) announceLeadership();
}

void Server::announceLeadership() {
    std::cout << sName << " announcing leadership." << std::endl;
    for (auto& peerPort : peerPorts) {
        sendMessage("LEADER", peerPort);
    }
}

void Server::receiveMessage(const std::string& message, int clientSock) {
    if (message == "ELECTION") {
        sendMessage("OK", ntohs(clientInfo.sin_port)); // Respond to election initiator
        initiateElection(); // Start a new election if this server has a higher port
    } else if (message == "LEADER") {
        isLeader = false;
        std::cout << sName << " acknowledges new leader." << std::endl;
    }
    // Implement handling OK messages and deciding leadership based on responses
}

void Server::sendMessage(const std::string& message, uint16_t port) {
    int sock = createSocketAndConnect(port);
    if (sock != -1) {
        send(sock, message.c_str(), message.length(), 0);
        close(sock);
    }
}

int Server::createSocketAndConnect(uint16_t port) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        return -1;
    }
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Assuming all servers are on localhost

    if (connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        close(sock);
        return -1;
    }
    return sock;
}
