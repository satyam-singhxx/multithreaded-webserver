cmake_minimum_required(VERSION 3.10)
project(MultithreadedServer)

set(CMAKE_CXX_STANDARD 17)

# Include directories
include_directories(include)

# Source files
file(GLOB SOURCES src/*.cpp)

# Executable
add_executable(MultithreadedServer ${SOURCES})