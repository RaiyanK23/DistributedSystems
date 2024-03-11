#ifndef TSESERVER_H
#define TSESERVER_H
#include "Server.h"

class TSEServer : public Server
{
public:
    TSEServer(const uint16_t portNumber, const std::string& serverName);
    ~TSEServer();
};
endif