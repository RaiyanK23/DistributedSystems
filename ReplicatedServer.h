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
#include <sys/socket.h> // For socket(), AF_INET, SOCK_DGRAM
#include <netinet/in.h> // For sockaddr_in, htonl(), htons()
#include <arpa/inet.h> // For inet_addr()
#include <unistd.h>
#include <map>
#include <string>
#include <utility> // For std::pair

template <typename ServerType>
class ReplicatedServer {
private:
    std::vector<std::unique_ptr<ReplicaManager<ServerType>>> replicaManagers;
    size_t currentReplicaIndex;
    std::map<size_t, std::pair<std::string, uint16_t>> peerInfo;
    int listeningPort; 
    size_t myId;
    bool electionInProgress;
    bool isLeader;

public:
    ReplicatedServer(size_t numReplicas, uint16_t startPort, const std::string& serverName);

    // void gossip();

    // void startGossipProtocol();

    void announceLeadership();


    void sendElectionMessage(size_t targetId);


    void messageListener();


    int createSocket();

    void handleIncomingMessage(const std::string& message, int listenSock, struct sockaddr_in* senderAddr);

    void initiateElectionProcess();
};

#include "ReplicatedServer.tpp"
