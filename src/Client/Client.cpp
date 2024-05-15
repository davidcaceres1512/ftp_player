#include "Client.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/select.h>
#include <algorithm>

Client::Client(const std::string& serverIP, int port) : serverIP(serverIP) {
     // Initialize the port for each SocketInfo object in connections
    SocketInfo initialConnection;
    initialConnection.port = port;
    connections.push_back(initialConnection);
}

Client::~Client() {
    // Close all opened sockets when destroying the object
    for (SocketInfo& socketInfo : connections) {
        close(socketInfo.sockfd);
    }
}

void Client::error(const std::string& errorMessage) {
    std::cerr << "Error: " << errorMessage << std::endl;
    exit(1);
}

int Client::createSocket() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error("Error opening socket");
    }
    
    return sockfd;
}

void Client::connectSocket(const std::string& serverIP, SocketInfo sockInf) {
    struct sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(sockInf.port);
    if (inet_pton(AF_INET, serverIP.c_str(), &serverAddr.sin_addr) <= 0) {
        error("Invalid or not supported IP address");
    }

    if (::connect(sockInf.sockfd, reinterpret_cast<struct sockaddr*>(&serverAddr), sizeof(serverAddr)) < 0) {
        error("Error connecting to server");
    }
}

void Client::connect(const std::string& serverIP, int port){
    // Create a new SocketInfo object
    SocketInfo socketInfo;
    socketInfo.sockfd = createSocket();
    socketInfo.port   = port;

    connectSocket(serverIP, socketInfo);
    
    connections.push_back(socketInfo);
}


void Client::send(int sockfd, const std::string& data) {
    if (write(sockfd, data.c_str(), data.length()) < 0) {
        error("Error sending data");
    }
}

std::string Client::receive(int sockfd, int bufferSize) {
    char response[bufferSize];
    ssize_t bytesRead = read(sockfd, response, bufferSize - 1);

    if (bytesRead < 0) {
        // Error reading server response
        response[0] = '\0';
        error("Error reading server response");
    } else {
        response[bytesRead] = '\0';
    }

    return std::string(response);
}

void Client::receiveAndWriteToFile(int sockfd, int bufferSize, std::ofstream& file) {
    char buffer[bufferSize];
    ssize_t bytesRead;
    while ((bytesRead = read(sockfd, buffer, bufferSize - 1)) > 0) {
        file.write(buffer, bytesRead);
    }
}

void Client::receiveAndConcatenateToString(int sockfd, int bufferSize, std::string& concatenatedData) {
    char buffer[bufferSize];
    ssize_t bytesRead;
    while ((bytesRead = read(sockfd, buffer, bufferSize - 1)) > 0) {
        concatenatedData.append(buffer, bytesRead); // Agregar solo los bytes leídos
    }
}

void Client::closeConnection(int sockfd) {
    // Find the SocketInfo object corresponding to the given sockfd
    std::vector<SocketInfo>::iterator it = std::find_if(connections.begin(), connections.end(), [sockfd](const SocketInfo& socketInfo) {
        return socketInfo.sockfd == sockfd;
    });

    if (it != connections.end()) {
        // Close the socket
        close(sockfd);
        // Remove the closed socket from the connections vector
        connections.erase(it);
    }

}

bool Client::isConnected(int sockfd) const {
    return std::find_if(connections.begin(), connections.end(), 
        [sockfd](const SocketInfo& socketInfo) {
            return socketInfo.sockfd == sockfd;
        }) != connections.end();
}

void Client::selectLoop() {
    fd_set readfds;
    int maxfd = -1;
    for (const SocketInfo& socketInfo : connections) {
        maxfd = std::max(maxfd, socketInfo.sockfd);
    }

    while (true) {
        FD_ZERO(&readfds);
        for (const SocketInfo& socketInfo : connections) {
            FD_SET(socketInfo.sockfd, &readfds);
        }

        int activity = select(maxfd + 1, &readfds, NULL, NULL, NULL);
        if (activity < 0) {
            error("Error in select");
        }

        for (const SocketInfo& socketInfo : connections) {
            if (FD_ISSET(socketInfo.sockfd, &readfds)) {
                // This socket is ready for reading
                std::cout << "Socket " << socketInfo.sockfd << " is ready to read" << std::endl;
                // Perform read operations here
            }
        }
    }
}

// Setter for serverIP
void Client::setServerIP(const std::string& serverIP) {
    this->serverIP = serverIP;
}

// Getter for serverIP
std::string Client::getServerIP() const {
    return serverIP;
}

// Setter for port
void Client::setPort(int sockfd, int port) {
    // Find the SocketInfo object corresponding to the given sockfd
    auto it = std::find_if(connections.begin(), connections.end(), [sockfd](const SocketInfo& socketInfo) {
        return socketInfo.sockfd == sockfd;
    });

    if (it != connections.end()) {
        // Update the port for the SocketInfo object
        it->port = port;
    }
}

// Getter for port
int Client::getPort(int sockfd) const {
    // Find the SocketInfo object corresponding to the given sockfd
    auto it = std::find_if(connections.begin(), connections.end(), [sockfd](const SocketInfo& socketInfo) {
        return socketInfo.sockfd == sockfd;
    });

    if (it != connections.end()) {
        // Return the port for the SocketInfo object
        return it->port;
    } else {
        return -1;
    }
}

// Getter for sockfd based on port
int Client::getSocketByPort(int port) const {
    // Find the SocketInfo object corresponding to the given port
    auto it = std::find_if(connections.begin(), connections.end(), [port](const SocketInfo& socketInfo) {
        return socketInfo.port == port;
    });

    if (it != connections.end()) {
        // Return the sockfd for the SocketInfo object
        return it->sockfd;
    } else {
        return -1; // Or any other error value indicating that the port was not found
    }
}

