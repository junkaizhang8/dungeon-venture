cmake_minimum_required(VERSION 3.10)

project(DungeonVenture)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(BUILD_EDITOR "Build the editor" OFF)
option(ENGINE_DEBUG "Enable debug mode for the engine" OFF)
if(ENGINE_DEBUG)
    message(STATUS "Engine debug mode enabled")
endif()

# Add subdirectories
add_subdirectory(engine)
if(BUILD_EDITOR)
    add_subdirectory(editor)
endif()