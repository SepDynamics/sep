#!/bin/bash
set -e

# Create build directory if it doesn't exist
mkdir -p build

# Configure only if CMakeCache.txt doesn't exist
if [ ! -f build/CMakeCache.txt ]; then
    echo "Configuring build..."
    cmake -Bbuild -S. -DCMAKE_BUILD_TYPE=Debug
fi

# Always build
echo "Building..."
cmake --build build

# Run tests
echo "Running tests..."
cd build
ctest --output-on-failure