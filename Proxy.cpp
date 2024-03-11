#include "Proxy.h"

Proxy::Proxy(const uint16_t portNumber, const std::string& serverName) : Server(portNumber, serverName) {}

void Proxy::handleConnection(const int clientSock)
{
    close(clientSock);
}

Proxy::~Proxy(){}