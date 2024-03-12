#include <iostream>
#include <vector>
#include <thread>
#include <mutex> // For thread synchronization
#include <chrono> // For time-related functions
#include <random> // For generating random numbers
#include <algorithm> // For std::shuffle
#include "Proxy.h"
#include "NYSEServer.h"

// Base class for server instances
class ServerInstance {
public:
    virtual void acceptConnection() = 0;
    // Add other common server operations here
};

// Replica manager class
template <typename ServerType>
class ReplicaManager : public ServerInstance {
private:
    ServerType server;
    std::thread thread;
    std::mutex mutex; // For thread synchronization
    bool running;

public:
    ReplicaManager(uint16_t portNumber, const std::string& serverName) : server(portNumber, serverName), running(true) {
        thread = std::thread(&ReplicaManager<ServerType>::acceptConnection, this);
    }

    ~ReplicaManager() {
        running = false;
        if (thread.joinable()) {
            thread.join();
        }
    }

    void acceptConnection() override {
        while (running) {
            server.acceptConnection();
            // Simulate processing time
            std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Adjust as needed
        }
    }

    std::mutex& getMutex() { return mutex; }
    // Add other server-specific methods as needed
};

// Replicated server class
template <typename ServerType>
class ReplicatedServer {
private:
    std::vector<std::unique_ptr<ReplicaManager<ServerType>>> replicaManagers;
    size_t currentReplicaIndex;

public:
    ReplicatedServer(size_t numReplicas, uint16_t startPort, const std::string& serverName) {
        currentReplicaIndex = 0;
        for (size_t i = 0; i < numReplicas; ++i) {
            replicaManagers.emplace_back(std::make_unique<ReplicaManager<ServerType>>(startPort + i, serverName + "_Replica_" + std::to_string(i)));
        }
    }

    // Gossip protocol to exchange information among replica managers
    void gossip() {
        std::vector<size_t> indices(replicaManagers.size());
        std::iota(indices.begin(), indices.end(), 0); // Fill with 0, 1, 2, ..., n-1
        std::random_device rd;
        std::mt19937 gen(rd());
        std::shuffle(indices.begin(), indices.end(), gen); // Shuffle indices

        for (size_t i = 0; i < indices.size(); ++i) {
            std::unique_lock<std::mutex> lock(replicaManagers[indices[i]]->getMutex());
            // Exchange information with replica at indices[i]
            // For simplicity, let's just print the index of the replica being gossiped with
            std::cout << "Gossiping with replica " << indices[i] << std::endl;
        }
    }

    // Start the gossip protocol in a separate thread
    void startGossipProtocol() {
        std::thread gossipThread(&ReplicatedServer::gossip, this);
        gossipThread.detach();
    }
};

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
