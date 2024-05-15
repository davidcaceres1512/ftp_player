# Add source files
set(SOURCES
        main.cpp
#        ${CMAKE_SOURCE_DIR}/src/Client/Client.cpp
#        ${CMAKE_SOURCE_DIR}/src/FTPClient/FTPClient.cpp
#        ${CMAKE_SOURCE_DIR}/src/VLCPlayer/VLCPlayer.cpp
#        ${CMAKE_SOURCE_DIR}/src/UARTCommunication/UARTCommunication.cpp
        )
      
add_executable(ftp_player ${SOURCES})

# Find necessary libraries

# Thread
find_package(Threads REQUIRED)
if (THREADS_FOUND)
    target_link_libraries(ftp_player PRIVATE Threads::Threads)
endif ()

# libVLC
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
find_package(VLC REQUIRED)
if (VLC_FOUND)
    target_include_directories(ftp_player PRIVATE ${VLC_INCLUDE_DIR})
    target_link_libraries(ftp_player PRIVATE ${VLC_LIBRARY})
else()
    message(FATAL_ERROR "libVLC not found.")
endif()

#add customized libraries
target_link_libraries(ftp_player PRIVATE Client FTPClient UARTCommunication VLCPlayer)



# Establecer los directorios de inclusión
target_include_directories(ftp_player PUBLIC
        #${CMAKE_SOURCE_DIR}/include/Client/
        #${CMAKE_SOURCE_DIR}/include/FTPClient/
        #${CMAKE_SOURCE_DIR}/include/UARTCommunication/
        #${CMAKE_SOURCE_DIR}/include/VLCPlayer/
        #${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Client/
        ${CMAKE_CURRENT_SOURCE_DIR}/include/FTPClient/
        )

        

# Establecer las opciones de compilación
target_compile_options(ftp_player PRIVATE
        -Wall
        -Wextra
        )