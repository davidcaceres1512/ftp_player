#ifndef FTPCLIENT_H
#define FTPCLIENT_H

#include <string>
#include "Client.h"
#define CONTROL_PORT 21

class FTPClient {
private:
    Client client;// object Client composition 
    std::string serverIP;
    int const controlPort = 21;//port 21 for control and command
    std::string username;
    std::string password;
    std::string remoteDirectory;
    std::string localDirectory;

    int bufSizeResponse;

    enum class FTPCommand {
    USER,
    PASS,
    CWD,
    PASV,
    RETR,
    LS,
    QUIT
    };

    enum class FileToken {
    PERMISSION =  1,
    USER,
    GROUP,
    SIZE,
    MONTH,
    DAY,
    TIME,
    NAME,
    };

    std::vector<std::string> ftpCommands;

    void error(const std::string& errorMessage);
    void connectToServer();
    std::string receiveResponse(int port = CONTROL_PORT);
    void sendCommand(const FTPCommand& command, const std::string& argument  = "", int port = CONTROL_PORT);
    void login();
    void quit();
    void CWDCommand();
    std::vector<std::string> LISTCommand(int port);
    std::string PASVCommand();
    void RETRCommand(const std::string& remoteFilename, int port);
    bool parsePASVResponse(const std::string& response, std::string& ip, int& port);
    std::vector<std::string> parseMusicFiles(const std::string& buffer, FileToken Token);
    bool parseLSResponse(const std::string& response);
    std::string trim(const std::string& str);
    void setBufSizeResponse(int bufferSize);
    void createLocalDirectory(const std::string& localDirectory);
    int getTotalTokens();

public:
    FTPClient(const std::string& serverIP, const std::string& username, const std::string& password,
               const std::string& remoteDirectory, const std::string& localDirectory);
    ~FTPClient();
    void downloadFile(const std::string& remoteFilename);
    std::vector<std::string> downloadAllMP3Files();
};

#endif
