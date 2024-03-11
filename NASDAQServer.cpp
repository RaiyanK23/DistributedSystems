#include "NASDAQServer.h"

NASDAQServer::NASDAQServer(const uint16_t& portNumber, const std::string& serverName) : Server(portNumber, serverName) {}

void NASDAQServer::handleConnection(const int clientSock)
{
    close(clientSock);
}

NASDAQServer::~NASDAQServer(){}