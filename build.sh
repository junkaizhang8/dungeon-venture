#!/bin/bash

# Create a build directory
mkdir -p build

# Build the project
cd build
cmake ..
cmake --build .
