#include "Proxy.h"
#include "NYSEServer.h"

int main()
{
    Proxy server;
    NYSEServer stockServer;
    std::thread(&NYSEServer::acceptConnection, &stockServer).detach();
    server.acceptConnection();

    return 0;
}