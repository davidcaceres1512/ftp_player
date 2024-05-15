#include <iostream>
#include <unistd.h>
#include <vector>
#include <chrono>
#include <thread>
#include "FTPClient.h"
#include "VLCPlayer.h"
#include "UARTCommunication.h" 

std::string serverIP = "192.168.1.50";
std::string username = "Fonestar";
std::string password = "odroid";
std::string remoteDirectory = "/Music/";
std::string localDirectory = "../Download/";

int main() {
    // Create an instance of FTPClient
    FTPClient ftpClient(serverIP, username, password, remoteDirectory, localDirectory);

    //Create an instance of UART
    UARTCommunication uart("/dev/ttySAC0", 115200);
    
    // Download all MP3 files located in the remote directory
    std::vector<std::string> downloadedFiles = ftpClient.downloadAllMP3Files();

    // Initialize VLCPlayer
    VLCPlayer player;
    if (!player.initialize()) {
        std::cerr << "Failed to initialize VLC player." << std::endl;
        return 1;
    }

    // Play the downloaded playlist
    if (!player.playPlaylist(downloadedFiles, localDirectory)) {
        std::cerr << "Failed to play playlist." << std::endl;
        return 1;
    }

    // Print playback time every second
    while (!player.isPlaybackFinished()) {
        // Obtener información actual de la música desde VLCPlayer
        std::string musicInfo = player.getCurrentMusicInfo();
        
        // Enviar datos a través de UARTCommunication
        uart.sendData(musicInfo.c_str());
        
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
