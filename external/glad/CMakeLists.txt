cmake_minimum_required(VERSION 3.4)

project(glad LANGUAGES C)

set(GLAD_SOURCES src/glad.c)

add_library(${PROJECT_NAME} STATIC ${GLAD_SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC include)