#include "gtest/gtest.h"
#include "UARTCommunication.h"

// Fixture para las pruebas de UARTCommunication
class UnitUARTCommunicationTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Configurar una instancia de UARTCommunication para las pruebas
        uart = new UARTCommunication("/dev/ttySAC0", 115200);
    }

    void TearDown() override {
        delete uart;
    }

    UARTCommunication* uart;
};

// Prueba para verificar el envío de datos
TEST_F(UnitUARTCommunicationTest, SendDataTest) {
    const char* testData = "Hello, UART!";
    EXPECT_TRUE(uart->sendData(testData));
}

// Prueba para verificar la recepción de datos
TEST_F(UnitUARTCommunicationTest, ReceiveDataTest) {
    char receiveBuffer[256];
    ssize_t bytesRead = uart->receiveData(receiveBuffer, sizeof(receiveBuffer));
    EXPECT_GE(bytesRead, 0);
}

// Prueba para verificar la función serialFlush
TEST_F(UnitUARTCommunicationTest, SerialFlushTest) {
    EXPECT_NO_THROW(uart->serialFlush());
}

