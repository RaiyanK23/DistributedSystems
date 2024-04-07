#include "ReplicatedServer.h"

template <typename ServerType>
ReplicatedServer<ServerType>::ReplicatedServer(size_t numReplicas, uint16_t startPort, const std::string& serverName) : listeningPort(startPort) {
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


template <typename ServerType>
int ReplicatedServer<ServerType>::createSocket() {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        std::cerr << "Error creating UDP socket: " << std::strerror(errno) << std::endl;
    }
    return sock;
}


template <typename ServerType>
void ReplicatedServer<ServerType>::messageListener() {
    int listenSock = createSocket();
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrSize = sizeof(clientAddr);

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); // Listen on all interfaces
    serverAddr.sin_port = htons(listeningPort); // Your listening port here

    if (bind(listenSock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Error binding UDP socket: " << std::strerror(errno) << std::endl;
        return;
    }

    while (true) {
        char buffer[1024] = {0};
        ssize_t bytesReceived = recvfrom(listenSock, buffer, sizeof(buffer), 0,
                                         (struct sockaddr *)&clientAddr, &addrSize);
        if (bytesReceived > 0) {
            std::string message(buffer, bytesReceived);
            handleIncomingMessage(message, listenSock, &clientAddr);
        }
    }
    close(listenSock);
}



template <typename ServerType>
void ReplicatedServer<ServerType>::sendElectionMessage(size_t targetId) {
    auto target = peerInfo[targetId];
    int sock = createSocket();
    if (sock == -1) return;

    struct sockaddr_in targetAddr;
    memset(&targetAddr, 0, sizeof(targetAddr));
    targetAddr.sin_family = AF_INET;
    targetAddr.sin_port = htons(target.second); // Target's listening port
    targetAddr.sin_addr.s_addr = inet_addr(target.first.c_str());

    std::string message = "ELECTION";
    sendto(sock, message.c_str(), message.size(), 0, (struct sockaddr *)&targetAddr, sizeof(targetAddr));
    close(sock);
}

template <typename ServerType>
void ReplicatedServer<ServerType>::announceLeadership() {
    for (const auto& peer : peerInfo) {
        if (peer.first != myId) {
            int sock = createSocket();
            if (sock == -1) continue;

            auto target = peerInfo[peer.first];
            struct sockaddr_in targetAddr;
            memset(&targetAddr, 0, sizeof(targetAddr));
            targetAddr.sin_family = AF_INET;
            targetAddr.sin_port = htons(target.second); // Target's listening port
            targetAddr.sin_addr.s_addr = inet_addr(target.first.c_str());

            std::string message = "LEADER";
            sendto(sock, message.c_str(), message.size(), 0, (struct sockaddr *)&targetAddr, sizeof(targetAddr));
            close(sock);
        }
    }

    
}

template <typename ServerType>
void ReplicatedServer<ServerType>::initiateElectionProcess() {
    std::cout << "Initiating election process." << std::endl;
    electionInProgress = true;  // Assuming there's an atomic bool flag indicating an election is in progress
    for (const auto& peer : peerInfo) {
        if (peer.first > myId) {  // Only send to peers with a higher ID
            sendElectionMessage(peer.first);
        }
    }

    // Setup a timeout for responses or assuming leadership if no higher ID responds
    // This could involve setting a timer and checking if any responses were received
    // For simplicity, let's assume immediate assumption of leadership if no higher IDs are present
    auto higherIdFound = std::any_of(peerInfo.begin(), peerInfo.end(), [this](const auto& peer){ return peer.first > myId; });
    if (!higherIdFound) {
        announceLeadership();
        isLeader = true;
        electionInProgress = false;
    }
    // Otherwise, wait for responses or a timeout to decide the next step
}


template <typename ServerType>
void ReplicatedServer<ServerType>::handleIncomingMessage(const std::string& message, int listenSock, struct sockaddr_in* senderAddr) {
    if (message == "ELECTION") {
        // Respond only if myId is higher than the sender's ID (assuming sender's ID is part of the message or inferred)
        const char* ipAddrStr = inet_ntoa(senderAddr->sin_addr); // Converts the IP to a string
        std::cout<<ipAddrStr<<std::endl;
        sendElectionMessage(strlen(ipAddrStr)); // Now correctly sending a C-string
        initiateElectionProcess();
    } else if (message == "LEADER") {
        // Acknowledge the new leader if the message is from a valid source
        std::cout << "New leader announced." << std::endl;
        isLeader = false;  // This server is not the leader
        electionInProgress = false;  // Election process concludes
        // Update internal state as needed to follow the new leader
    }
    // Implement logic for handling "OK" responses to election messages
}
