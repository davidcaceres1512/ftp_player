# Agregar todos los archivos fuente de la biblioteca
add_library(Client Client.cpp)

# Instalar la biblioteca (opcional)
install(TARGETS Client DESTINATION ${CMAKE_SOURCE_DIR}/lib)