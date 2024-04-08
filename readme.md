# Server Program

This program sets up a proxy server and an NYSE server, allowing clients to connect to them.

## Prerequisites

- C++ compiler (e.g., g++)
- Standard C++ library
- POSIX threads library (for multithreading)
(these are already included in the code)
- local installation of mongodb with 3 replicas running on ports 27017, 27018, 27019

## Compilation

Compile the program and start database replicas using the following commands:

```bash
g++ -std=c++14 NYSEServer.cpp Server.cpp main.cpp Proxy.cpp -lpthread $(pkg-config --cflags --libs libmongocxx)
```

```bash
 mongod --replSet rs0 --port 27017 --bind_ip localhost --dbpath ./srv/mongodb/rs0-0 --oplogSize 128
```

```bash
 mongod --replSet rs0 --port 27018 --bind_ip localhost --dbpath ./srv/mongodb/rs0-1 --oplogSize 128
```

```bash
 mongod --replSet rs0 --port 27019 --bind_ip localhost --dbpath ./srv/mongodb/rs0-2 --oplogSize 128
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
