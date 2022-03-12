cmake_minimum_required(VERSION 3.7)
project(ERHook VERSION 0.1 DESCRIPTION "Elden Ring Memory Hook")

set(DEFAULT_BUILD_TYPE "Release")
option(ERHOOK_STATIC_LIB "Build static library" ON)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

set(
    SOURCES
    src/ERHook.cpp
    include/ERHook.hpp
    include/Offsets.h
    include/Info.h
)

if(ERHOOK_STATIC_LIB)
    add_library(${PROJECT_NAME} STATIC ${SOURCES})
else()
    add_library(${PROJECT_NAME} SHARED ${SOURCES})
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER include/ERHook.h)

target_include_directories(${PROJECT_NAME} PRIVATE include)
target_include_directories(${PROJECT_NAME} PRIVATE src)


# Configure Demo
set(DEMO_SOURCES src/Main.cpp)
add_executable(${PROJECT_NAME}_DEMO ${DEMO_SOURCES})
target_include_directories(${PROJECT_NAME}_DEMO PRIVATE include)
target_link_libraries(${PROJECT_NAME}_DEMO ${PROJECT_NAME})