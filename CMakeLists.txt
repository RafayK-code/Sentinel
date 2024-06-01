cmake_minimum_required(VERSION 3.3)
project("Sentinel")

set(CMAKE_CXX_STANDARD 17)

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH})

add_subdirectory(external/spdlog)

set(SOURCES 
    src/Dbg.cpp
    src/ECS/Scene.cpp 
    src/ECS/ComponentPool.cpp
    src/ECS/ComponentChunk.cpp
)

set(HEADERS 
    include/Core.h 
    include/Dbg.h
    include/ECS/ECS.h
    include/ECS/Scene.h
    include/ECS/Types.h
    include/ECS/ComponentPool.h
    include/ECS/ComponentChunk.h
    include/ECS/SceneView.h
)

add_compile_definitions(ENGINE_EXPORT)
add_compile_definitions(SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)

add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>"
                                                                  "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

target_include_directories(${PROJECT_NAME} PUBLIC external/spdlog/include)
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog)

add_library(${PROJECT_NAME}::Sentinel ALIAS ${PROJECT_NAME})