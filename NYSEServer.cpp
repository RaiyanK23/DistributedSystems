#include "NYSEServer.h"

NYSEServer::NYSEServer(const uint16_t& portNumber, const std::string& serverName) : Server(portNumber, serverName) {}

void NYSEServer::handleConnection(const int clientSock)
{
    close(clientSock);
}

NYSEServer::~NYSEServer(){}