#ifndef PROXY_H
#define PROXY_H
#include "Server.h"
#include <vector>

class Proxy : public Server
{
public:
    Proxy(const uint16_t& portNumber, const std::string& serverName);
    ~Proxy();

    void handleConnection(const int clientSock) override;

private:
    void handlePostRequest(const int clientSock, const std::string& request);
    int createSocket(const std::string& ipAddress, const std::vector<int>& ports);
};
#endif