#ifndef NYSESERVER_H
#define NYSESERVER_H
#include "Server.h"

class NYSEServer : public Server
{
public:
    NYSEServer(const uint16_t portNumber, const std::string& serverName);
    ~NYSEServer();

    void handleConnection(const int clientSock) override;
};
#endif