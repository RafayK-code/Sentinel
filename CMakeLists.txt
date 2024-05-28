cmake_minimum_required(VERSION 3.3)
project("Sentinel")

set(CMAKE_CXX_STANDARD 17)

set(SOURCES 
    src/main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})