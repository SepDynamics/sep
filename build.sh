#!/bin/bash
# Simple build script that actually works

set -uo pipefall

echo "Building SEP Engine..."
sudo  rm -rf .cache .codechecker CMakeCache.txt output CMakeFiles Makefile 
sudo rm -rf /sep/.Trash-1000
mkdir .cache output .codechecker/output build
totxt.save

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
        -DCMAKE_EXPORT_COMPILE_COMMANDS=TRUE \
        -DCMAKE_CXX_COMPILER=clang++-15 \
        -DSEP_USE_CUDA=ON
    
    # Build
    cp /project/build/compile_commands.json /host/
    ninja
    
    # Copy to host
    echo "Copying build artifacts to host..."
    cp -r /project/build /host/
    echo "Build complete!"
'
echo "Checking build results..."
if [ -f "build/sep_workbench" ]; then
    echo "SUCCESS: SEP Workbench executable found at build/sep_workbench"
    ls -la build/sep_workbench
    echo "OANDA Trader structure created in src/apps/oanda_trader/"
    ls -la src/apps/oanda_trader/ 2>/dev/null || true
    OANDA_API_KEY="$OANDA_API_KEY" OANDA_ACCOUNT_ID="$OANDA_ACCOUNT_ID" ./build/sep_workbench
    # sudo scripts/install_sep_service.sh
else
    echo "ERROR: SEP executable not found"
    echo "Checking for other executables..."
    ls -la build/ 2>/dev/null || true
    exit 1
fi
