#include "NYSEServer.h"
#include <mongocxx/client.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <algorithm>
#include <iostream>
#include <vector>

constexpr int MAX_BUFFER_SIZE = 2048;

// Constructor for NYSEServer class
NYSEServer::NYSEServer(const uint16_t &portNumber, const std::string &serverName) : Server(portNumber, serverName) {}

// Function to handle incoming connections
void NYSEServer::handleConnection(const int clientSock)
{
    // Buffer to store data received from the proxy
    std::vector<char> buffer(MAX_BUFFER_SIZE);

    // Receive data from the proxy
    ssize_t bytesRead = recv(clientSock, buffer.data(), buffer.size(), 0);
    if (bytesRead == -1)
    {
        // Error receiving data
        perror("Error receiving data from proxy");
        close(clientSock);
        return;
    }

    // Convert the received data to a string
    std::string requestData(buffer.data(), bytesRead);

    // Print received data
    std::cout << "Data received from proxy: " << requestData << std::endl;

    // Create a JSON string with the received data
    std::ostringstream oss;
    oss << requestStockMongo(requestData); // Call function to fetch stock data from MongoDB

    std::string jsonData = oss.str();

    // Now 'jsonData' contains the received data in JSON format

    // Send the JSON data back to the client
    send(clientSock, jsonData.c_str(), jsonData.size(), 0);

    // Close the client socket
    close(clientSock);
}

// Function to fetch stock data from MongoDB
std::string NYSEServer::requestStockMongo(const std::string stockName)
{

    std::string jsonString;
    try
    {
        // Establish connection to MongoDB replicated database
        mongocxx::uri uri("mongodb://localhost:27017");
        mongocxx::client client(uri);

        // Access database and collection
        auto db = client["CPSC599_Project"];
        auto collection = db["StockMetrics"];

        // Create filter for querying stock data
        auto filter = bsoncxx::builder::stream::document{} << "StockName" << stockName << bsoncxx::builder::stream::finalize;
        
        // Query MongoDB and iterate through results
        auto cursor = collection.find({filter});
        for (auto&& doc : cursor) {
            // Convert BSON documents to JSON strings and concatenate
            jsonString += bsoncxx::to_json(doc);
        }
    }
    catch (const std::exception &e)
    {
        // Handle errors
        std::cout << "Exception: " << e.what() << std::endl;
    }

    return jsonString;
}

// Destructor for NYSEServer class
NYSEServer::~NYSEServer() {}
