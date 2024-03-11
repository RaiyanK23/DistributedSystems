#ifndef PROXY_H
#define PROXY_H
#include "Server.h"


class Proxy : public Server
{
public:
    Proxy(const uint16_t portNumber, const std::string& serverName);
    ~Proxy();

    void handleConnection(const int clientSock) override;
};
#endif