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
NYSEServer::NYSEServer(const uint16_t &portNumber, const std::string &serverName) : Server(portNumber, serverName) {}

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
   
    

    std::cout << "Data received from proxy: " << requestData << std::endl;

    // Create a JSON string with the received data
    std::ostringstream oss;
    //oss << "{\"stock\": \"" << requestData << "\"}";

    oss << requestStockMongo(requestData);

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

std::string NYSEServer::requestStockMongo(const std::string stockName)
{

    std::string jsonString;
    try
    {
        

        mongocxx::uri uri("mongodb://localhost:27018");
        mongocxx::client client(uri);

        auto db = client["CPSC599_Project"];
        auto collection = db["StockMetrics"];

        
        auto filter = bsoncxx::builder::stream::document{} << "StockName" << stockName << bsoncxx::builder::stream::finalize;
        auto cursor = collection.find({filter});

          // String to store concatenated JSON strings

        // Assuming cursor is an iterable container of bsoncxx::document::view instances
        for (auto&& doc : cursor) {
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

NYSEServer::~NYSEServer() {}