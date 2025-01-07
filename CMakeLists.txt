cmake_minimum_required(VERSION 3.10)

project(DungeonVenture)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(BUILD_EDITOR "Build the editor" OFF)

# Add subdirectories
add_subdirectory(engine)
if(BUILD_EDITOR)
    add_subdirectory(editor)
endif()