#ifndef MOCK_UART_COMMUNICATION_H
#define MOCK_UART_COMMUNICATION_H

#include "UARTCommunication.h"
#include <string>
// mock class for UARTCommunication
class MockUARTCommunication : public UARTCommunicationInterface {
public:
    MockUARTCommunication(const std::string& device, int baudRate);
    virtual ~MockUARTCommunication();
    virtual bool sendData(const char* data) override;
    virtual ssize_t receiveData(char* buffer, size_t bufferSize) override;
    virtual void serialFlush() override;

private:
    // add any member needed
};

#endif // MOCK_UART_COMMUNICATION_H
