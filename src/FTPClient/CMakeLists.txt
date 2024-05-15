# Agregar todos los archivos fuente de la biblioteca
add_library(FTPClient 
        FTPClient.cpp
        ../Client/Client.cpp)

# Establecer los directorios de inclusión para los encabezados
target_include_directories(FTPClient PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ../Client/
        )

# Instalar la biblioteca (opcional)
install(TARGETS FTPClient DESTINATION ${CMAKE_SOURCE_DIR}/../lib)