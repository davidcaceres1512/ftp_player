#include "UARTCommunication.h"
#include <fcntl.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <iostream>

UARTCommunication::UARTCommunication(const char *device, int baudRate) : baudRate_(baudRate), fd_(-1) {
    int     status;
    // Open UART device
    fd_ = open(device, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd_ == -1) {
        perror("Failed to open UART device");
        return;
    }

    // Configure UART settings
    struct termios options;
    tcgetattr(fd_, &options);
    cfsetispeed(&options, baudRate_);
    cfsetospeed(&options, baudRate_);
    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    options.c_oflag &= ~OPOST;

    options.c_cc [VMIN]  =   0 ;
    options.c_cc [VTIME] = 100 ;	// Ten seconds (100 deciseconds)

    tcsetattr(fd_, TCSANOW, &options);

    ioctl (fd_, TIOCMGET, &status);

    status |= TIOCM_DTR ;
    status |= TIOCM_RTS ;

    ioctl (fd_, TIOCMSET, &status);

    usleep (10000) ;	// 10mS

    // Flush input and output buffers
    tcflush(fd_, TCIFLUSH);
}

UARTCommunication::~UARTCommunication() {
    if (fd_ != -1) {
        close(fd_);
    }
}

bool UARTCommunication::sendData(const char *data) {
    ssize_t bytesWritten = write(fd_, data, strlen(data));
    if (bytesWritten == -1) {
        perror("Failed to send data over UART");
        return false;
    }
    return true;
}

ssize_t UARTCommunication::receiveData(char *buffer, size_t size) {
    ssize_t bytesRead = read(fd_, buffer, size);
    if (bytesRead == -1) {
        perror("Failed to receive data from UART");
    }
    return bytesRead;
}

void UARTCommunication::serialFlush() {
    tcflush(fd_, TCIOFLUSH);
}


