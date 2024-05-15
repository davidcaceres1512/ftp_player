#include "MockUARTCommunication.h"
#include <cstdio>
#include <cstring>
// Implementación de los métodos de MockUARTCommunication

MockUARTCommunication::MockUARTCommunication(const std::string& device, int baudRate) {
    // Inicializa la simulación de la comunicación UART
}

MockUARTCommunication::~MockUARTCommunication() {
    // Libera los recursos de la simulación de la comunicación UART
}

bool MockUARTCommunication::sendData(const char* data) {
    // Simula el envío de datos
    return true; // Supongamos que siempre se envía correctamente en el mock
}

ssize_t MockUARTCommunication::receiveData(char* buffer, size_t bufferSize) {
    // Simula la recepción de datos
    std::string testData = "Hello, UART!";
    size_t len = testData.size();
    if (len > bufferSize) len = bufferSize;
    memcpy(buffer, testData.c_str(), len);
    return len;
}

void MockUARTCommunication::serialFlush() {
    // Simula serialFlush
}
