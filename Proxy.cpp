#include "Proxy.h"

Proxy::Proxy(const uint16_t& portNumber, const std::string& serverName) : Server(portNumber, serverName) {}

void Proxy::handleConnection(const int clientSock)
{
    // Handle the incoming request based on the HTTP method
    char buffer[2048]; // Buffer to store the incoming request
    memset(buffer, 0, sizeof(buffer));

    // Receive the request from the client
    ssize_t bytesRead = recv(clientSock, buffer, sizeof(buffer), 0);
    if (bytesRead == -1) {
        perror("Error receiving data from client");
        close(clientSock);
        return;
    }

    // Parse the HTTP request
    std::string request(buffer, bytesRead);

    // Check if it's a POST request
    if (request.find("POST") == 0) {
        // Handle the POST request
        handlePostRequest(clientSock, request);
    } else {
        // Unsupported method
        std::string response = "HTTP/1.1 405 Method Not Allowed\r\n\r\n";
        send(clientSock, response.c_str(), response.size(), 0);
        close(clientSock);
    }
}

void Proxy::handlePostRequest(const int clientSock, const std::string& request)
{
    // Extract the data from the request body
    size_t pos = request.find("\r\n\r\n");
    if (pos != std::string::npos) {
        std::string requestBody = request.substr(pos + 4);
        // Log the received data
        std::cout << "Data received from client: " << requestBody << std::endl;
        

    std::vector<int> portsToTry = {8010, 8011, 8012};
    int serverSock = createSocket("192.168.48.128", portsToTry);
    if (serverSock == -1) {
        // All connection attempts failed, send HTTP failure message to the client
        std::string response = "HTTP/1.1 503 Service Unavailable\r\n\r\n";
        send(clientSock, response.c_str(), response.size(), 0);
        close(clientSock);
        return;
    }

    // Send the POST data to the server on port 8010
    send(serverSock, requestBody.c_str(), requestBody.size(), 0);

    // Receive and process the JSON response from the server on port 8010
    char buffer[1024];
    std::string jsonResponse;
    int bytesReceived;
    while ((bytesReceived = recv(serverSock, buffer, sizeof(buffer), 0)) > 0) {
        jsonResponse.append(buffer, bytesReceived);
    }

    // Close the connection to the server on port 8010
    close(serverSock);

    // Construct the HTTP response with the JSON data
    std::string response = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n" + jsonResponse;

    // Send the HTTP response back to the client
    send(clientSock, response.c_str(), response.size(), 0);


    } else {
        // Malformed request
        std::string response = "HTTP/1.1 400 Bad Request\r\n\r\n";
        send(clientSock, response.c_str(), response.size(), 0);
    }

    // Close the client socket
    close(clientSock);
}

int Proxy::createSocket(const std::string& ipAddress, const std::vector<int>& ports)
{
    int serverSock = -1;
    struct sockaddr_in serverAddr;

    // Loop through each port and try to connect
    for (auto port : ports) {
        // Attempt to create a socket
        serverSock = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSock == -1) {
            std::cerr << "Error creating server socket: " << std::strerror(errno) << std::endl;
            continue;  // Try the next port
        }

        // Specify the server address and port
        memset(&serverAddr, 0, sizeof(serverAddr));
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = inet_addr(ipAddress.c_str());
        serverAddr.sin_port = htons(port);

        // Attempt to connect to the server
        if (connect(serverSock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == 0) {
            // Connection successful, break out of the loop
            break;
        } else {
            // Connection failed, close the socket and try the next port
            std::cerr << "Error connecting to server on port " << port << ": " << std::strerror(errno) << std::endl;
            close(serverSock);
            serverSock = -1;
        }
    }

    return serverSock;
}

Proxy::~Proxy(){}