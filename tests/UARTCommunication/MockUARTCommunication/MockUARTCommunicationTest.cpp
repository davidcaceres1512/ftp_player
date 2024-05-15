#include "gtest/gtest.h"
#include "MockUARTCommunication.h"

// Fixture para las pruebas de UARTCommunication
class MockUARTCommunicationTest : public ::testing::Test {
protected:
    void SetUp() override {
        uart = new MockUARTCommunication("/dev/ttySAC0", 115200);
    }

    void TearDown() override {
        delete uart;
    }

    UARTCommunicationInterface* uart;
};

// Prueba para verificar el envío de datos
TEST_F(MockUARTCommunicationTest, SendDataTest) {
    const char* testData = "Hello, UART!";
    EXPECT_TRUE(uart->sendData(testData));
}

// Prueba para verificar la recepción de datos
TEST_F(MockUARTCommunicationTest, ReceiveDataTest) {
    char receiveBuffer[256];
    ssize_t bytesRead = uart->receiveData(receiveBuffer, sizeof(receiveBuffer));
    EXPECT_GE(bytesRead, 0);
}

// Prueba para verificar la función serialFlush
TEST_F(MockUARTCommunicationTest, SerialFlushTest) {
    EXPECT_NO_THROW(uart->serialFlush());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}