#pragma once

#include <thread>
#include <mutex>
#include "ServerInstance.h"

template <typename ServerType>
class ReplicaManager : public ServerInstance {
private:
    ServerType server;
    std::thread thread;
    std::mutex mutex;
    bool running;

public:
    ReplicaManager(uint16_t portNumber, const std::string& serverName);

    ~ReplicaManager();

    void acceptConnection() override;

    std::mutex& getMutex();
};

#include "ReplicaManager.tpp"
