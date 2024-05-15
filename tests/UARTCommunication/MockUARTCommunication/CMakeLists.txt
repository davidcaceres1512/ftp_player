# Agregar el ejecutable de las pruebas
add_executable(MockUARTCommunicationTest MockUARTCommunicationTest.cpp MockUARTCommunication.cpp)

# Incluir la carpeta de las bibliotecas del proyecto
include_directories(${CMAKE_SOURCE_DIR}/include/UARTCommunication/)
include_directories(${googletest_SOURCE_DIR}/googletest/include)
include_directories(./)
# Enlazar el ejecutable de las pruebas con GoogleTest y la biblioteca del proyecto
target_link_libraries(MockUARTCommunicationTest PRIVATE gtest gtest_main UARTCommunication)

# Agregar la prueba como una prueba de CTest
add_test(
    NAME MockUARTCommunicationTest
    COMMAND MockUARTCommunicationTest
)