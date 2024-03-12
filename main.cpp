#include "ReplicatedServer.h"
#include "Proxy.h"
#include "NYSEServer.h"

int main() {
    const size_t NUM_REPLICAS = 3;

    // Create a replicated proxy server
    ReplicatedServer<Proxy> replicatedProxy(NUM_REPLICAS, 8080, "Proxy");
    replicatedProxy.startGossipProtocol();

    // Create a replicated NYSE server
    ReplicatedServer<NYSEServer> replicatedNYSE(NUM_REPLICAS, 8010, "NYSE_Server");
    replicatedNYSE.startGossipProtocol();

    // Main thread will continue to run other tasks or wait for user input
    // ...

    return 0;
}
