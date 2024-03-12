template <typename ServerType>
ReplicatedServer<ServerType>::ReplicatedServer(size_t numReplicas, uint16_t startPort, const std::string& serverName) {
    currentReplicaIndex = 0;
    for (size_t i = 0; i < numReplicas; ++i) {
        replicaManagers.emplace_back(std::make_unique<ReplicaManager<ServerType>>(startPort + i, serverName + "_Replica_" + std::to_string(i)));
    }
}

template <typename ServerType>
void ReplicatedServer<ServerType>::gossip() {
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

template <typename ServerType>
void ReplicatedServer<ServerType>::startGossipProtocol() {
    std::thread gossipThread(&ReplicatedServer::gossip, this);
    gossipThread.detach();
}
