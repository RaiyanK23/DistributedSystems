template <typename ServerType>
ReplicaManager<ServerType>::ReplicaManager(uint16_t portNumber, const std::string& serverName) : server(portNumber, serverName), running(true) {
    thread = std::thread(&ReplicaManager<ServerType>::acceptConnection, this);
}

template <typename ServerType>
ReplicaManager<ServerType>::~ReplicaManager() {
    running = false;
    if (thread.joinable()) {
        thread.join();
    }
}

template <typename ServerType>
void ReplicaManager<ServerType>::acceptConnection() {
    while (running) {
        server.acceptConnection();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

template <typename ServerType>
std::mutex& ReplicaManager<ServerType>::getMutex() {
    return mutex;
}
