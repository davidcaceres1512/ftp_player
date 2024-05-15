#include "FTPClient.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <regex>
#include <sys/stat.h> // For mkdir function



FTPClient::FTPClient(const std::string& serverIP,  
                     const std::string& username, const std::string& password,
                     const std::string& remoteDirectory, const std::string& localDirectory)
    : serverIP(serverIP),
    username(username),password(password), 
    remoteDirectory(remoteDirectory), 
    localDirectory(localDirectory),
    client(serverIP, controlPort) {
    
    // Here it returns the data received from sockets of the Client class
    bufSizeResponse = 4096;

    // Matriz de cadenas que representa los comandos FTP
    ftpCommands = {
        "USER ",
        "PASS ",
        "CWD ",
        "PASV",
        "RETR ",
        "LIST",
        "QUIT"
    };

    // Check and create local directory
    createLocalDirectory(localDirectory);

    connectToServer();
    login();
    CWDCommand();

}

FTPClient::~FTPClient(){
    quit();
}

void FTPClient::error(const std::string& errorMessage) {
    std::cerr << "Error: " << errorMessage << std::endl;
    exit(1);
}

void FTPClient::connectToServer() {
    client.connect(serverIP, controlPort);
    
    std::string response = receiveResponse();
    if (response.substr(0, 3) != "220") {
        error("Error connecting to server");
    }
}

std::string FTPClient::receiveResponse(int port) {
    return client.receive(client.getSocketByPort(port), bufSizeResponse); 
}

void FTPClient::sendCommand(const FTPCommand& command, const std::string& argument, int port) {
    std::string commandStr = ftpCommands[static_cast<int>(command)];
    if (!argument.empty()) {
        commandStr += argument;
    }
    commandStr += "\r\n"; // Append carriage return and line feed
    client.send(client.getSocketByPort(port), commandStr);
}

void FTPClient::login() {
    sendCommand(FTPCommand::USER, username);
    std::string response = receiveResponse();

    if (response.substr(0, 3) != "331") {
        error("Username error");
    }

    sendCommand(FTPCommand::PASS, password);
    response = receiveResponse();

    if (response.substr(0, 3) != "230") {
        error("Password error");
    }
}

void FTPClient::CWDCommand() {
    sendCommand(FTPCommand::CWD, remoteDirectory);
    std::string response = receiveResponse();

    if (response.substr(0, 3) != "250") {
        error("Error changing remote directory");
    }
}

std::string FTPClient::trim(const std::string& str) {
    // Find the first non-whitespace character
    size_t start = str.find_first_not_of(" \t\r\n");
    // Find the last non-whitespace character
    size_t end = str.find_last_not_of(" \t\r\n");
    if (start == std::string::npos) {
        // If the string is all whitespace, return an empty string
        return "";
    }
    // Return the substring containing the non-whitespace characters
    return str.substr(start, end - start + 1);
}


std::vector<std::string> FTPClient::LISTCommand(int port) {
    sendCommand(FTPCommand::LS);
    std::string response = receiveResponse();
    std::cout<<"response: "<<response<<std::endl;//debug
    if (response.substr(0, 3) != "150") {
        error("Error listing files");
    }

    std::vector<std::string> fileList;

    std::string concatString;

    client.receiveAndConcatenateToString(client.getSocketByPort(port), bufSizeResponse, concatString);

    response = receiveResponse();
    if (response.substr(0, 3) != "226") {
        error("Error completing data transfer");
    }

    fileList = parseMusicFiles(concatString, FileToken::NAME);

    for(const auto& str : fileList) {//debug
        std::cout << str << std::endl;
    }
    
    return fileList;
}

std::string FTPClient::PASVCommand(){
    sendCommand(FTPCommand::PASV);
    std::string response = receiveResponse();

    if (response.substr(0, 3) != "227") {
        error("Error obtaining data transfer port");
    }

    return response;
}

bool FTPClient::parsePASVResponse(const std::string& response, std::string& ip, int& port) {
    // Analyze the response to extract the IP address and passive port
    std::string responseStr(response);
    std::regex regexPattern("\\((\\d+),(\\d+),(\\d+),(\\d+),(\\d+),(\\d+)\\)");
    std::smatch match;

    if (std::regex_search(responseStr, match, regexPattern) && match.size() == 7) {
        std::string ipPart1 = match[1].str();
        std::string ipPart2 = match[2].str();
        std::string ipPart3 = match[3].str();
        std::string ipPart4 = match[4].str();
        std::string portPart1 = match[5].str();
        std::string portPart2 = match[6].str();

        ip = ipPart1 + "." + ipPart2 + "." + ipPart3 + "." + ipPart4;
        port = std::stoi(portPart1) * 256 + std::stoi(portPart2);

        std::cout << "ipAddress: " << ip << ", port: " << port << std::endl; 
        return true; // Parsing successful
    } else {
        // Unable to extract IP address and port from the FTP server response
        error("Error parsing FTP server response");
        return false; // Parsing failed
    }
}

void FTPClient::RETRCommand(const std::string& remoteFilename, int port) {
    sendCommand(FTPCommand::RETR, remoteFilename);
    std::string response = receiveResponse();
    if (response.substr(0, 3) != "150") {
        error("Error downloading file");
    }

    std::ofstream file(localDirectory + "/" + remoteFilename, std::ios::binary);
    if (!file.is_open()) {
        error("Error creating local file");
    }

    client.receiveAndWriteToFile(client.getSocketByPort(port), bufSizeResponse, file);

    file.close();

    response = receiveResponse();
    if (response.substr(0, 3) != "226") {
        error("Error completing data transfer");
    }
}

void FTPClient::downloadFile(const std::string& remoteFilename) {
    
    std::string response = PASVCommand();

    // Get IP address and port for data transfer
    std::string ip;
    int port;

    if (!parsePASVResponse(response, ip, port)) {
        error("Error parsing PASV response");
    }

    client.connect(serverIP, port);

    RETRCommand(remoteFilename, port);

    close(client.getSocketByPort(port));
}

std::vector<std::string> FTPClient::downloadAllMP3Files() {
    // Connect to data port once
    std::string response = PASVCommand();

    // Get IP address and port for data transfer
    std::string ip;
    int port;

    if (!parsePASVResponse(response, ip, port)) {
        error("Error parsing PASV response");
    }

    client.connect(ip, port);

    // List all files in the remote directory
    std::vector<std::string> fileList = LISTCommand(port);

    close(client.getSocketByPort(port));

    // Iterate over the file list and download MP3 files
    for (const std::string& filename : fileList) {
        // Check if the file is an MP3 file
        if (filename.size() >= 4 && filename.substr(filename.size() - 4) == ".mp3") {
            
            response = PASVCommand();

            if (!parsePASVResponse(response, ip, port)) {
                error("Error parsing PASV response");
            }

            client.connect(ip, port);

            // Download the MP3 file
            RETRCommand(filename, port); 

            // Close data socket
            client.closeConnection(client.getSocketByPort(port));        
        }
    }

    return fileList;
}

void FTPClient::createLocalDirectory(const std::string& localDirectory) {
    struct stat st;
    if (stat(localDirectory.c_str(), &st) != 0) {
        // Directory does not exist, create it
        if (mkdir(localDirectory.c_str(), 0777) != 0) {
            // Error creating directory
            std::cerr << "Error creating local directory: " << strerror(errno) << std::endl;
            exit(1);
        }
    } else if (!S_ISDIR(st.st_mode)) {
        // Path exists but is not a directory
        std::cerr << "Local path is not a directory" << std::endl;
        exit(1);
    }
}

// Function to get the total number of tokens in the FileToken enum
int FTPClient::getTotalTokens() {
    return (int) FileToken::NAME;
}

std::vector<std::string> FTPClient::parseMusicFiles(const std::string& buffer, FileToken Token) {
    std::vector<std::string> musicFiles;
    std::istringstream iss(buffer);
    std::string line;
    while (std::getline(iss, line)) {
        std::istringstream lineStream(line);
        std::string token;
        for (int i = 0; i < getTotalTokens(); ++i) {
            lineStream >> token;
            if (i == (int) Token) { 
                break;
            }
        }
        // The rest of the line is the file name
        std::string filename;
        std::getline(lineStream, filename);
        // Trim leading and trailing whitespaces
        filename = trim(filename);
        // Add the file name to the vector
        musicFiles.push_back(filename);
    }
    return musicFiles;
}

void FTPClient::setBufSizeResponse(int bufSizeResponse) {
    this->bufSizeResponse = bufSizeResponse;
}

void FTPClient::quit() {
    sendCommand(FTPCommand::QUIT);
    std::string response = receiveResponse();
}
