# Agregar todos los archivos fuente de la biblioteca
add_library(UARTCommunication UARTCommunication.cpp)

# Establecer los directorios de inclusión para los encabezados
target_include_directories(UARTCommunication PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        )

# Instalar la biblioteca (opcional)
install(TARGETS UARTCommunication DESTINATION ${CMAKE_SOURCE_DIR}/../lib)
message(STATUS "CMAKE_INSTALL_PREFIX: ${googletCMAKE_INSTALL_PREFIX}")