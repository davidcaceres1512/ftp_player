cmake_minimum_required(VERSION 3.14)
project(ftp_player)

set(CMAKE_CXX_STANDARD 11)

# Include GoogleTest
include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest
        GIT_TAG release-1.12.0
        GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(googletest)

# Include subdirectories to achieve modularity
add_subdirectory(app)
add_subdirectory(src)
add_subdirectory(include)

# Configure tests
enable_testing()
add_subdirectory(tests)
