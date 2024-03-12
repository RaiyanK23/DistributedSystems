#pragma once

#include <vector>
#include <memory>
#include <mutex>
#include <thread>
#include <iostream>
#include <random>
#include <algorithm>
#include "ServerInstance.h"
#include "ReplicaManager.h"

template <typename ServerType>
class ReplicatedServer {
private:
    std::vector<std::unique_ptr<ReplicaManager<ServerType>>> replicaManagers;
    size_t currentReplicaIndex;

public:
    ReplicatedServer(size_t numReplicas, uint16_t startPort, const std::string& serverName);

    void gossip();

    void startGossipProtocol();
};

#include "ReplicatedServer.tpp"
