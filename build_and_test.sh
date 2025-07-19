#!/bin/bash
set -e

# Ensure we are in the project root directory
cd /sep

# Clean up ALL build artifacts from both root and build directory
echo "Cleaning up previous build artifacts..."
rm -rf build/
rm -f CMakeCache.txt
rm -rf CMakeFiles/
rm -f build.ninja
rm -f compile_commands.json
rm -f CTestTestfile.cmake
rm -f DartConfiguration.tcl
rm -f cmake_install.cmake
rm -f sep-config.cmake

# Create fresh build directory
echo "Creating fresh build directory..."
mkdir -p build
cd build

# Configure the project using CMake with explicit paths
echo "Configuring build in $(pwd)..."
export CUDA_HOST_COMPILER=/usr/bin/clang++-15
cmake -S .. -B . -GNinja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=/usr/bin/clang++-15

# Verify build files were created in the right place
echo "Checking for build.ninja in $(pwd)..."
if [ ! -f "build.ninja" ]; then
    echo "ERROR: build.ninja not found in build directory!"
    echo "Contents of build directory:"
    ls -la
    exit 1
fi

# Build the project
echo "Building project..."
ninja

# Run the tests
echo "Running tests..."
ctest --output-on-failure