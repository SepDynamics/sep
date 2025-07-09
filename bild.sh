#!/bin/bash
set -e

# Clean up the build directory
echo "Cleaning build directory..."
rm -rf /sep/sep_build
mkdir -p /sep/sep_build
cd /sep/sep_build

# Reconfigure with CMake
echo "Running CMake..."
cmake .. -DSEP_BUILD_TESTS=ON -DSEP_WORKBENCH_DEMO=ON -DSEP_BUILD_QUANTUM=ON

echo "Build configuration complete!"


cd /sep 
export CC=gcc-14 CXX=g++-14 
make -j$(nproc) 2>&1 | tee /sep/build_log.txt