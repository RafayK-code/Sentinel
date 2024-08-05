cmake_minimum_required(VERSION 3.3)
project("Sentinel")

set(CMAKE_CXX_STANDARD 17)

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH})

add_subdirectory(external/spdlog)
add_subdirectory(external/glfw)
add_subdirectory(external/glad)
add_subdirectory(external/glm)

set(SNTL_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SNTL_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src)

file(GLOB_RECURSE SRC_FILES
    "${SNTL_SRC}/*.cpp"
)

file(GLOB_RECURSE INCLUDE_FILES
    "${SNTL_INCLUDE}/*.h"
)

include(cmake/source_group.cmake)

option(SNTL_SHARED_LIB "Build engine as a shared lib" OFF)
set(LIB_TYPE STATIC)

if(SNTL_SHARED_LIB)

    set(LIB_TYPE SHARED)
endif()

add_library(${PROJECT_NAME} ${LIB_TYPE} ${SRC_FILES} ${INCLUDE_FILES})

if(SNTL_SHARED_LIB)
    add_compile_definitions(SNTL_EXPORT)
    target_compile_definitions(${PROJECT_NAME} PUBLIC SNTL_SHARED_LIB)
endif()

add_compile_definitions(SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)

target_compile_definitions(${PROJECT_NAME} PUBLIC 
    $<$<CONFIG:Debug>:SNTL_DEBUG>
    $<$<CONFIG:Release>:SNTL_RELEASE>
    $<$<CONFIG:RelWithDebInfo>:SNTL_RELWITHDEBINFO>
    $<$<CONFIG:MinSizeRel>:SNTL_MINSIZEREL>
)

target_compile_definitions(${PROJECT_NAME} PUBLIC
    $<$<PLATFORM_ID:Windows>:SNTL_PLATFORM_WINDOWS>
    $<$<PLATFORM_ID:Darwin>:SNTL_PLATFORM_MAC>
    $<$<PLATFORM_ID:Linux>:SNTL_PLATFORM_LINUX>
)

option(ENABLE_LOGGING "Enable logging in all modes besides min size release" ON)
option(FORCE_LOGGING "Enable logging in all modes" OFF)

if(ENABLE_LOGGING)
    target_compile_definitions(${PROJECT_NAME} PUBLIC 
        $<$<CONFIG:Debug>:SNTL_ENABLE_LOGGING>
        $<$<CONFIG:Release>:SNTL_ENABLE_LOGGING>
        $<$<CONFIG:RelWithDebInfo>:SNTL_ENABLE_LOGGING>
    )
endif()

if(FORCE_LOGGING)
    target_compile_definitions(${PROJECT_NAME} PUBLIC 
        $<$<CONFIG:Debug>:SNTL_ENABLE_LOGGING>
        $<$<CONFIG:Release>:SNTL_ENABLE_LOGGING>
        $<$<CONFIG:RelWithDebInfo>:SNTL_ENABLE_LOGGING>
        $<$<CONFIG:MinSizeRel>:SNTL_ENABLE_LOGGING>
    )
endif()

target_include_directories(${PROJECT_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>"
                                                                  "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

target_include_directories(${PROJECT_NAME} PUBLIC external/spdlog/include)
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog)

target_include_directories(${PROJECT_NAME} PUBLIC external/glfw/include)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

target_include_directories(${PROJECT_NAME} PUBLIC external/glad/include)
target_link_libraries(${PROJECT_NAME} PRIVATE glad)

target_include_directories(${PROJECT_NAME} PUBLIC external/glm)

add_library(${PROJECT_NAME}::Sentinel ALIAS ${PROJECT_NAME})