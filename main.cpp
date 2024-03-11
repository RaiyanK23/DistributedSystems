#include "Proxy.h"
#include "NYSEServer.h"

int main()
{

    Proxy proxyServer(8080, "Proxy");
    NYSEServer stockServer(8010, "NYSEServer");
    std::thread(&NYSEServer::acceptConnection, &stockServer).detach();
    proxyServer.acceptConnection();

    return 0;
}