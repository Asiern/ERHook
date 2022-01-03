cmake_minimum_required(VERSION 3.7)
project(ERHook VERSION 0.1 DESCRIPTION "Elden Ring Memory Hook")

set(DEFAULT_BUILD_TYPE "Release")
option(ERHOOK_STATIC_LIB "Build static library" ON)

set(
    SOURCES
    src/ERHook.cpp
)

if(ERHOOK_STATIC_LIB)
    add_library(${PROJECT_NAME} STATIC ${SOURCES})
else()
    add_library(${PROJECT_NAME} STATIC ${SOURCES})
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER include/ERHook.h)

target_include_directories(${PROJECT_NAME} PRIVATE include)
target_include_directories(${PROJECT_NAME} PRIVATE src)
