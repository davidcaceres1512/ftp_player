#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <vector>

class Client {
private:

    struct SocketInfo {
        int sockfd;
        int port;
    };

    std::string serverIP;
    std::vector<SocketInfo> connections;// socket descriptors
    int bufferSize;

    void error(const std::string& errorMessage);

public:
    Client(const std::string& serverIP, int port);
    ~Client();
	
    int createSocket(); 
    void connectSocket(const std::string& serverIP, SocketInfo sockInf);
    void connect(const std::string& serverIP, int port);
    void send(int sockfd, const std::string& data);
    std::string receive(int sockfd, int bufferSize);
    void receiveAndWriteToFile(int sockfd, int bufferSize, std::ofstream& file);
    void receiveAndConcatenateToString(int sockfd, int bufferSize, std::string& concatenatedData);
    bool isConnected(int sockfd) const;
    std::vector<int> getActiveSockets() const;
    void closeConnection(int sockfd);
    void setServerIP(const std::string& serverIP);
    std::string getServerIP() const;
    void setPort(int sockfd, int port);
    int getPort(int sockfd) const;
    int getSocketByPort(int port) const;
    void selectLoop();
};

#endif // CLIENT_H
