#ifndef NYSESERVER_H
#define NYSESERVER_H
#include "Server.h"
#include <vector>
#include <string>
#include <sstream>
// #include <mongocxx/client.hpp>
// #include <mongocxx/instance.hpp>

class NYSEServer : public Server
{
public:
    NYSEServer(const uint16_t& portNumber, const std::string& serverName);
    ~NYSEServer();

    void handleConnection(const int clientSock) override;
    

private:
    //mongocxx::client _mongoClient; // MongoDB client instance

    // Function to initialize the MongoDB client
    //void initializeMongoClient();
    std::string requestStockMongo(const std::string stockName);
};
#endif