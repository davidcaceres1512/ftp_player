#ifndef UART_COMMUNICATION_H
#define UART_COMMUNICATION_H

#include <unistd.h>

// Interface 
class UARTCommunicationInterface {
public:
    virtual ~UARTCommunicationInterface() {}
    virtual bool sendData(const char* data) = 0;
    virtual ssize_t receiveData(char* buffer, size_t bufferSize) = 0;
    virtual void serialFlush() = 0;
};

class UARTCommunication : public UARTCommunicationInterface {
private:
    int fd_; // File descriptor for UART device
    int baudRate_;

public:
    UARTCommunication(const char *device, int baudRate);
    virtual ~UARTCommunication();

    virtual void serialFlush() override;
    virtual bool sendData(const char *data) override;
    virtual ssize_t receiveData(char *buffer, size_t size) override;
};

#endif // UART_COMMUNICATION_H