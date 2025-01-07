#!/bin/bash

# Clean the build directory
if [[ $1 == "clean" ]]; then
    rm -rf build
    echo "Build directory cleaned."
    exit 0
fi

# Clear the CMake build type cache
if [[ $1 == "clear-cache" ]]; then
    rm -f .build_type
    echo "CMake build type cache cleared."
    exit 0
fi

# Display help message
if [[ $1 == "help" ]]; then
    echo "Usage: $0 [all|editor|clean|clear-cache|help]"
    exit 0
fi

# Check if a build type is passed by the user
# If not, check if a build type is cached in .build_type file
# If not, use the default build type: all
if [[ -z "$1" && -f .build_type ]]; then
    BUILD_TYPE=$(cat .build_type)
else
    BUILD_TYPE=${1:-all}

    if [[ $BUILD_TYPE != "all" && $BUILD_TYPE != "game" && $BUILD_TYPE != "editor" ]]; then
        echo "Invalid build type: $BUILD_TYPE"
        echo "Usage: $0 [all|editor|clean|clear-cache|help]"
        exit 1
    fi
fi

# Cache the build type
echo $BUILD_TYPE > .build_type

# Create a build directory
mkdir -p build
cd build

# Build the project
if [[ "$BUILD_TYPE" == "all" ]]; then
    echo "Building the project..."
    cmake -DBUILD_EDITOR=ON ..
elif [[ "$BUILD_TYPE" == "editor" ]]; then
    echo "Building the editor..."
    cmake -DBUILD_EDITOR=ON ..
fi

cmake --build .

echo "Build complete."