#!/bin/bash
# Simple build script that actually works

set -e

echo "Building SEP Engine..."

# Ensure Docker image is built
docker build -t sep-engine-builder .

# Build and copy executable
docker run --gpus all --rm -v $(pwd):/host sep-engine-builder bash -c '
    cd /project
    rm -rf build
    mkdir build
    cd build
    
    # Configure
    cmake .. -G Ninja \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_C_COMPILER=clang-15 \
        -DCMAKE_CXX_COMPILER=clang++-15 \
        -DSEP_USE_CUDA=ON
    
    # Build
    ninja
    
    # Copy to host
    echo "Copying build artifacts to host..."
    cp -r /project/build /host/
    echo "Build complete!"
'

echo "Checking build results..."
if [ -f "build/sep" ]; then
    echo "SUCCESS: SEP executable found at build/sep"
    ls -la build/sep
else
    echo "ERROR: SEP executable not found"
    exit 1
fi