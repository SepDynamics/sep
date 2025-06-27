#!/bin/bash

# Exit on error
set -e

# Clean build directory
echo "Cleaning build directory..."
rm -rf /sep/cmake-make
mkdir -p /sep/cmake-make
cd /sep/cmake-make

# Configure with GCC
echo "Configuring with GCC..."
cd /sep/cmake-make
cmake -S /sep -B /sep/cmake-make \
  -DCMAKE_C_COMPILER=/usr/bin/gcc-14 \
  -DCMAKE_CXX_COMPILER=/usr/bin/g++-14 \
  -DWITH_CYCLES_DEVICE_OPTIX=ON \
  -DOPTIX_ROOT_DIR=/sep/cmake-make/optix_headers

# Check if CMake generated the Makefile
echo "Checking build directory..."
ls -la /sep/cmake-make
if [ -f "/sep/cmake-make/Makefile" ]; then
  echo "Makefile found, proceeding with build..."
  # Build
  echo "Building..."
  make -j$(nproc)
else
  echo "ERROR: Makefile not found in /sep/cmake-make"
  echo "CMake configuration may have failed"
  exit 1
fi

echo "Build completed successfully!"