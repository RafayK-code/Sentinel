cmake_minimum_required(VERSION 3.3)
project("Sentinel")

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(external/spdlog)

set(SOURCES 
    src/main.cpp
    src/Dbg.cpp
)

set(HEADERS 
    include/Core.h 
    include/Dbg.h
)

add_compile_definitions(ENGINE_EXPORT)
add_compile_definitions(SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)

add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>"
                                                                  "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

target_include_directories(${PROJECT_NAME} PRIVATE external/spdlog/include)
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog)