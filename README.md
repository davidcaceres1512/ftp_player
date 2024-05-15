
[![LinkedIn][linkedin-shield]][linkedin-url]



<!-- PROJECT LOGO -->
<br />
<div align="center">
  <a href="https://github.com/github_username/repo_name">
    <img src="img/logo.png" alt="Logo" width="80" height="80">
  </a>

<h3 align="center">FTP PLAYER</h3>

  <p align="center">
    FTP music player
    <br />
    <a href="https://github.com/davidcaceres1512/ftp_player"><strong>Explore the docs »</strong></a>
    <br />
  </p>
</div>



<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#testing">Testing</a></li>
    <li><a href="#deployment on an  SBC">Deployment on an  SBC</a></li>
    <li><a href="#contact">Contact</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project


This project is an FTP music player designed to facilitate playing music files stored on a remote FTP server. Using technologies like C++, CMake, and VLC, the player provides a simple interface to efficiently download, manage, and play MP3 files.

<p align="right">(<a href="#readme-top">back to top</a>)</p>



### Built With


* [![UbuntuMate][UbuntuMate]][UbuntuMate-url]
* [![cpp][cpp]][cpp-url]
* [![cmake][cmake]][cmake-url]
* [![Googletest][Googletest]][Googletest-url]


<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- GETTING STARTED -->
## Getting Started

### Prerequisites

This is an example of how to list things you need to use the software and how to install them.
* **cmake**
  ```sh
  sudo apt update
  sudo apt install cmake
  ```
  After installation, you can verify if CMake installed correctly by running:
  ```bash
  cmake --version
  ```
  It should display the installed CMake version on your system. Now you're ready to use CMake on Ubuntu.

  > Ensure you have at least version 3.14; otherwise, cmake won't allow you to install the executable.

* **vlc**
  ```sh
  sudo apt update
  sudo apt install vlc
  ```
* **libvlc**

  To install libVLC (VLC library), you can do so with the following command:
  ```sh
  sudo apt install libvlc-dev
  ```
> ### Note
> * **googletest:** This prerequisite is installed through a CMake script "CMakeLists.txt" using FetchContent, so you shouldn't worry about installing it manually.

### Installation

1. Clone the repo
   ```bash  
   git clone https://github.com/davidcaceres1512/ftp_player.git
   ```
2. Compile and create libraries using CMake
   ```sh
   mkdir build
   cd build/
   cmake ..
   cmake --build .
   ```
3. Run the program
   ```sh
   ./app/ftp_player
   ```

  > ### Note
  > Installation can be interrupted if you don't meet the prerequisites since I've created a script in CMake to check if you have all the necessary dependencies.

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- USAGE EXAMPLES -->
## Usage

Navigate from the project's root folder to the **app** folder and open the **main.cpp** file to modify it.

To modify the main program and see our changes without modifying the CMakeLists.txt files, follow these steps:

1. Navigate to the **build** folder.

2. Modify the **main.cpp** file.

3. Run the following command to apply the changes
    ```bash
    make clean
    make
    ```

4. Run the binary

    ```bash
    ./app/ftp_player
    ```
### Example 1
Download a file from the remote FTP server's folder.

> Note that all songs located in the remote **Music** folder are: 
> 1. 30 Seconds to Mars - The Kill.mp3
> 2. Boris Brejcha - Purple Noise.mp3
> 3. The Killers - Sweet Talk.mp3

> Files are preferably downloaded to the project's main path **Download** to locate them quickly. You can change the path by modifying the **localDirectory** variable in the **main.cpp** script.

```cpp
#include <iostream>
#include <unistd.h>
#include "FTPClient.h"

std::string serverIP = "192.168.1.50";
std::string username = "Fonestar";
std::string password = "odroid";
std::string remoteDirectory = "/Music/";
std::string localDirectory = "../Download/";
std::string availableSong1 = "30 Seconds to Mars - The Kill.mp3";
std::string availableSong2 = "Boris Brejcha - Purple Noise.mp3";
std::string availableSong3 = "The Killers - Sweet Talk.mp3";

int main() {
    // Create an instance of FTPClient
    FTPClient ftpClient(serverIP, username, password, remoteDirectory, localDirectory);
 
    // Download a MP3 file located in the remote directory
    ftpClient.downloadFile(availableSong1);

    return 0;
}
```

### Example 2
Download all files located in the remote FTP server's folder and display the downloaded files in the console.

```cpp
#include <iostream>
#include <unistd.h>
#include <vector>
#include "FTPClient.h"


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

    // Print each element of the vector
    for (const auto& file : downloadedFiles) {
        std::cout << file << std::endl;
    }

    return 0;
}
```

### Example 3
Download all files located in the remote FTP server's folder, play the downloaded MP3 files, and display the song title and playback time (in minutes and seconds) on the console.

```cpp
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
        // Get current music information from VLCPlayer
        std::string musicInfo = player.getCurrentMusicInfo();
        
        // Print the current music information and playback time to the terminal
        std::cout << musicInfo << std::endl;
        
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
```

### Example 4
Download all files located in the remote FTP server's folder, play the downloaded MP3 files, and send the song title and playback time (in minutes and seconds) via the UART protocol.

> This example is found in the main.cpp file and is the purpose of this project.



<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- TESTING -->
## Testing
Instructions for running the test suite usando googletest y cmake.

> These instructions are valid after completing the installation steps.

### End-to-End Tests

Navigate from the project's root folder to the build folder
```cmd
cd build/
ctest
```

### SuitName Tests
Navigate from the project's main directory, then go to the **build** folder
```cmd
cd build/
ctest -R MockUARTCommunicationTest 
ctest -R UnitUARTCommunicationTest 
```

### Note
> If you want to see the detailed results, you have to open the **LastTest.log** file.
> Navigate from the project's root folder to the folder containing the log and open it.
> ```cmd
> cd build/Testing/Temporary/
> vim  LastTest.log
> ```

> At the moment, I have made progress in performing Unit Testing and Mocks for the UARTCommunication class.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- Deployment on an SBC -->
## Deployment on an SBC
The deployment depends on the Single Board Computer (SBC) you are using. In my case, I'm using an ODROID XU 4.

A functionality of the project is to send data via UART protocol to another device. To achieve this, we must rely on the datasheet provided by the manufacturer to make the correct connections, remembering that each SBC operates with different voltage technologies such as TTL 5v, CMOS 3.3v, or 1.8v.

### UART Port

To know how to connect the UART pins, it is recommended to visit the following link [Odroid GPIO].

Most peripherals that have pins on the CON10 Header connector operate at 1.8v level. The **UART_0.RXD** port (Pin #6) and **UART_0.TXD** port (Pin #8) operate at 1.8v. Be careful, it is recommended to connect a logic level shifter to connect it to an Arduino or STM32.

![odroid connection img][odroid connection img]

From the main **main.cpp** code, be careful when assigning the UART port.

> ```cpp
> //Create an instance of UART
> UARTCommunication uart("/dev/ttySAC0", 115200);
> ```

This parameter should be documented in your BSP datasheet. Refer to the following figure for details.

![odroid serial img][odroid serial img]

### Digital Analizer

To verify that the UART port is functional and sending data, I have used a digital logic analyzer from [DSLogic]. Click on the link for more information and to download the software.

The steps are as follows:

1. Connect a probe to the UART port of the SBC.
![dslogic connection][dslogic connection]

2. Connect the DSLogic to the PC and open the software.

3. Configure the port of the probe and choose the corresponding decoding.

    ![dslogic conf][dslogic conf]

### Results

Finally, we can see the result in the following images.

The DSLogic has decoded the frame:

*Name: 30 Seconds to Mars - The Kill.mp3 Playback time: 00:11*

![logic out 1][logic out 1]

![logic out 2][logic out 2]

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- CONTACT -->
## Contact

David Caceres - [@linkedin](https://www.linkedin.com/in/david-caceres-santi/) - davidxcaceres1@gmail.com

Project Link: [https://github.com/davidcaceres1512/ftp_player](https://github.com/davidcaceres1512/ftp_player)

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- MARKDOWN LINKS & img -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->


[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://www.linkedin.com/in/david-caceres-santi/

[Odroid GPIO]: https://wiki.odroid.com/odroid-xu4/hardware/expansion_connectors

[DSLogic]: https://www.dreamsourcelab.com/product/dslogic-series/

[odroid connection img]: img/odroid_connection.png
[odroid serial img]: img/odroid_serial_device.png
[dslogic connection]: img/dslogic_connection.png
[dslogic conf]: img/dslogic_conf.png
[logic out 1]: img/logic_output1.png
[logic out 2]: img/logic_output2.png


<!-- Built-with Badges -->
[UbuntuMate]: https://img.shields.io/badge/Ubuntu%20Mate-%2387a556?style=for-the-badge&logo=ubuntu&logoColor=white&logoWidth=40
[UbuntuMate-url]:https://ubuntu-mate.org/

[cpp]: https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white
[cpp-url]: https://cplusplus.com/

[cmake]: https://img.shields.io/badge/CMake-064F8C?style=for-the-badge&logo=cmake&logoColor=white
[cmake-url]: https://cmake.org

[Googletest]: img/googletest-icon.svg
[Googletest-url]: https://github.com/google/googletest



