cmake_minimum_required(VERSION 3.3)
project("Sentinel")

set(CMAKE_CXX_STANDARD 17)

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH})

add_subdirectory(external/spdlog)

set(SNTL_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SNTL_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src)

file(GLOB CORE
    "${SNTL_INCLUDE}/*.h"
    "${SNTL_SRC}/*.cpp"
)

file(GLOB ECS 
    "${SNTL_INCLUDE}/ecs/*.h"
    "${SNTL_SRC}/ecs/*.cpp"
)

file(GLOB SYS
    "${SNTL_INCLUDE}/sys/*.h"
    "${SNTL_SRC}/sys/*.cpp"
)

add_compile_definitions(ENGINE_EXPORT)
add_compile_definitions(SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)

add_library(${PROJECT_NAME} SHARED ${CORE} ${ECS} ${SYS})

source_group("core" FILES ${CORE})
source_group("ecs" FILES ${ECS})
source_group("sys" FILES ${SYS})

target_include_directories(${PROJECT_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>"
                                                                  "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

target_include_directories(${PROJECT_NAME} PUBLIC external/spdlog/include)
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog)

add_library(${PROJECT_NAME}::Sentinel ALIAS ${PROJECT_NAME})