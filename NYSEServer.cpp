#include "NYSEServer.h"
constexpr int MAX_BUFFER_SIZE = 2048;
NYSEServer::NYSEServer(const uint16_t& portNumber, const std::string& serverName) : Server(portNumber, serverName) {}

void NYSEServer::handleConnection(const int clientSock)
{
    // Buffer to store data received from the proxy
    std::vector<char> buffer(MAX_BUFFER_SIZE);

    // Receive data from the proxy
    ssize_t bytesRead = recv(clientSock, buffer.data(), buffer.size(), 0);
    if (bytesRead == -1) {
        // Error receiving data
        perror("Error receiving data from proxy");
        close(clientSock);
        return;
    }

    // Convert the received data to a string
    std::string requestData(buffer.data(), bytesRead);
    std::cout << "Data received from proxy: " << requestData << std::endl;

    // Create a JSON string with the received data
    std::ostringstream oss;
    oss << "{\"stock\": \"" << requestData << "\"}";
    std::string jsonData = oss.str();

    // Now 'jsonData' contains the received data in JSON format

    // Send the JSON data back to the client
    send(clientSock, jsonData.c_str(), jsonData.size(), 0);

    // Close the client socket
    close(clientSock);
}

// void NYSEServer::initializeMongoClient() {
//     try {
//         // Initialize the MongoDB client instance
//         _mongoClient = mongocxx::client{mongocxx::uri{}};
//     } catch (const mongocxx::exception& e) {
//         std::cerr << "Error initializing MongoDB client: " << e.what() << std::endl;
//         // Handle initialization error
//     }
// }
NYSEServer::~NYSEServer(){}