#include "ReplicatedServer.h"
#include "Proxy.h"
#include "NYSEServer.h"
#include <mongocxx/instance.hpp>

int main() {
    const size_t NUM_REPLICAS = 3;

    mongocxx::instance inst{};


    // Create a replicated proxy server
    //  Proxy primaryProxy(8083, "primaryProxy");
    //  primaryProxy.acceptConnection();

    // NYSEServer primaryServer(8010, "NYSEServer");
    // primaryServer.acceptConnection();

    ReplicatedServer<Proxy> replicatedProxy(NUM_REPLICAS, 8080, "Proxy");
    

    // Create a replicated NYSE server
    ReplicatedServer<NYSEServer> replicatedNYSE(NUM_REPLICAS, 8010, "NYSE_Server");
    

    // Main thread will continue to run other tasks or wait for user input
    // ...

    return 0;
}
