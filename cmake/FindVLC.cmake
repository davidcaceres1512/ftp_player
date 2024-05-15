# Este script busca la biblioteca libVLC y define variables para su uso en el proyecto.

# Buscar la biblioteca libVLC
find_library(VLC_LIBRARY
    NAMES vlc
    PATH_SUFFIXES lib
)

# Verificar si se encontró la biblioteca libVLC
if(VLC_LIBRARY)
    # Encontrar la ruta a los archivos de cabecera de libVLC
    find_path(VLC_INCLUDE_DIR
        NAMES vlc/vlc.h
        PATH_SUFFIXES include
    )

    # Definir variables encontradas
    if(VLC_INCLUDE_DIR)
        set(VLC_FOUND TRUE)
        message(STATUS "Found libVLC: ${VLC_LIBRARY}")
        message(STATUS "libVLC include directory: ${VLC_INCLUDE_DIR}")
    else()
        message(WARNING "Found libVLC library, but couldn't find include directory.")
    endif()
else()
    message(WARNING "libVLC library not found.")
endif()