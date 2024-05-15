# Establecer los directorios de inclusión para los encabezados
target_include_directories(VLCPlayer PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        )

# libVLC
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
find_package(VLC REQUIRED)
if (VLC_FOUND)
    target_include_directories(VLCPlayer PRIVATE ${VLC_INCLUDE_DIR})
    #target_link_libraries(ftp_player PRIVATE ${VLC_LIBRARY})
else()
    message(FATAL_ERROR "libVLC not found.")
endif()
        
