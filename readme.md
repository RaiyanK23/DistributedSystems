# Server Program

This program sets up a proxy server and an NYSE server, allowing clients to connect to them.

## Prerequisites

- C++ compiler (e.g., g++)
- Standard C++ library
- POSIX threads library (for multithreading)
(these are already included in the code)
- local installation of mongodb with 3 replicas running on ports 27017, 27018, 27019

## Compilation

Compile the program using the following command:

```bash
g++ -std=c++14 NYSEServer.cpp Server.cpp main.cpp Proxy.cpp -lpthread $(pkg-config --cflags --libs libmongocxx)
```

## Run the Program
```bash
./a.out
```

# Testing the Server Connection with Telnet

Telnet can be used to test the connection to your server. Follow these steps to connect using Telnet:

1. **Open a Terminal/Command Prompt**: Open a terminal or command prompt on your computer.

2. **Connect to the Servers**:
   ```bash
   telnet localhost <server_port>
```
Server ports are set to 8080 and 8010 (check the code)