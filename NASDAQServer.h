#ifndef NASDAQSERVER_H
#define NASDAQSERVER_H
#include "Server.h"


class NASDAQServer : public Server
{
public:
    NASDAQServer(const uint16_t& portNumber, const std::string& serverName);
    ~NASDAQServer();

    void handleConnection(const int clientSock) override;
};
#endif