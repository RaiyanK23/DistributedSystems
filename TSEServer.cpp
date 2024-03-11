#include "TSEServer.h"

TSEServer::TSESERVER(const uint16_t& portNumber, const std::string& serverName) : Server(portNumber, serverName) {}

void TSESERVER::handleConnection(const int clientSock)
{
    close(clientSock);
}

TSEServer::~TSEServer(){}