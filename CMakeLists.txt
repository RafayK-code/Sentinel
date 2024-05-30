cmake_minimum_required(VERSION 3.3)
project("Sentinel")

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(external/spdlog)

set(SOURCES 
    src/main.cpp
    src/Core.h 
    src/Dbg.h 
    src/Dbg.cpp
)

add_compile_definitions(ENGINE_EXPORT)
add_compile_definitions(SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC external/spdlog/include)
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog)