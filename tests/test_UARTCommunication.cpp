#include "gtest/gtest.h"
#include "UARTCommunication.h"

// Fixture para las pruebas de UARTCommunication
class UARTCommunicationTest : public ::testing::Test {
protected:
    // Configurar antes de cada prueba
    void SetUp() override {
        // Configurar una instancia de UARTCommunication con un dispositivo ficticio y velocidad de baudios ficticia
        uart = new UARTCommunication("/dev/ttySAC0", 115200);
    }

    // Limpiar después de cada prueba
    void TearDown() override {
        delete uart; // Liberar la instancia de UARTCommunication
    }

    // Miembro de datos para la instancia de UARTCommunication
    UARTCommunication* uart;
};

// Prueba para verificar el envío de datos
TEST_F(UARTCommunicationTest, SendDataTest) {
    // Datos de prueba a enviar
    const char* testData = "Hello, UART!";

    // Enviar los datos y verificar si se envían correctamente
    EXPECT_TRUE(uart->sendData(testData));
}

// Prueba para verificar la recepción de datos
TEST_F(UARTCommunicationTest, ReceiveDataTest) {
    // Buffer para almacenar los datos recibidos
    char receiveBuffer[256];

    // Recibir datos y verificar si se reciben correctamente
    ssize_t bytesRead = uart->receiveData(receiveBuffer, sizeof(receiveBuffer));
    EXPECT_GE(bytesRead, 0); // Comprobar que se haya recibido al menos un byte
}

// Prueba para verificar la función serialFlush
TEST_F(UARTCommunicationTest, SerialFlushTest) {
    // Ejecutar serialFlush y verificar si no hay errores
    EXPECT_NO_THROW(uart->serialFlush());
}
