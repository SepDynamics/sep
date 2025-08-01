#!/bin/bash
# Simplified build script for running inside a container

# Allow overriding the container runtime via DOCKER_BIN.
DOCKER_BIN=${DOCKER_BIN:-docker}

set -uo pipefail

REBUILD=false
if [ "${1:-}" == "--rebuild" ]; then
    REBUILD=true
fi

echo "Building SEP Engine..."

if [ "$REBUILD" = true ]; then
    echo "Performing a full rebuild..."
    # Clean up previous build artifacts
    sudo rm -rf CMakeCache.txt CMakeFiles output Makefile build .cache .codechecker
    sleep 2
    clear
    sudo rm -rf /sep/.Trash-1000
    sleep 1
fi

mkdir -p output
mkdir -p build

# Ensure proper permissions for CodeChecker directories
USER_ID=$(id -u)
GROUP_ID=$(id -g)

# Fallback to native build if container runtime is unavailable
if ! "$DOCKER_BIN" info >/dev/null 2>&1; then
    echo "Container runtime $DOCKER_BIN not available. Building natively..."
    cd build
    cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Release -DSEP_USE_CUDA=OFF \
        -DCMAKE_CXX_COMPILER=clang++-15 -DCMAKE_C_COMPILER=clang-15
    ninja -k 0 2>&1 | tee ../output/build_log.txt
    cd ..
    exit 0
fi

# Build and setup development environment
CUDA_PREFIX="$(dirname "$(command -v nvcc 2>/dev/null)" 2>/dev/null | sed 's#/bin$##')"
if [ -z "$CUDA_PREFIX" ] || [ ! -d "$CUDA_PREFIX" ]; then
    CUDA_PREFIX=/usr/local/cuda
fi
if [ ! -d "$CUDA_PREFIX" ]; then
    CUDA_PREFIX=/usr
fi
"${DOCKER_BIN}" run --gpus all --rm \
    -v $(pwd):/sep \
    -e CUDA_HOME=$CUDA_PREFIX \
    -e CUDA_TOOLKIT_ROOT_DIR=$CUDA_PREFIX \
    -e CUDA_BIN_PATH=$CUDA_PREFIX/bin \
    -e CMAKE_CUDA_COMPILER=$CUDA_PREFIX/bin/nvcc \
    -e CUDAToolkit_ROOT=$CUDA_PREFIX \
    -e PATH=$CUDA_PREFIX/bin:${PATH} \
    -e LD_LIBRARY_PATH=$CUDA_PREFIX/lib64:/usr/lib/x86_64-linux-gnu:${LD_LIBRARY_PATH-} \
    sep-engine-builder bash -c '
    # Verify CUDA environment
    echo "Verifying CUDA environment..."
    echo "CUDA_HOME: $CUDA_HOME"
    echo "CUDA_TOOLKIT_ROOT_DIR: $CUDA_TOOLKIT_ROOT_DIR"
    echo "CUDA_BIN_PATH: $CUDA_BIN_PATH"
    echo "CMAKE_CUDA_COMPILER: $CMAKE_CUDA_COMPILER"
    ls -la $CUDA_HOME/bin/nvcc || echo "NVCC not found!"
    # Build as root - no user switching needed
    # Add exception for dubious ownership
    git config --global --add safe.directory '*'
    cd /sep
    cd build
    
    # Configure with include dependency tracking
    cmake .. -G Ninja \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_C_COMPILER=clang-15 \
        -DCMAKE_EXPORT_COMPILE_COMMANDS=TRUE \
        -DCMAKE_CXX_COMPILER=clang++-15 \
        -DCMAKE_CXX_FLAGS="${CMAKE_CXX_FLAGS} ${EXTRA_CXX_FLAGS:-}" \
        -DCMAKE_CUDA_FLAGS="${CMAKE_CUDA_FLAGS} ${EXTRA_CUDA_FLAGS:-}" \
        -DCUDAToolkit_ROOT=$CUDA_PREFIX \
        -DSEP_USE_CUDA=ON
    
    # Build with logging
    ninja -k 0 2>&1 | tee /sep/output/build_log.txt
    
    # Copy and fix compile_commands.json for IDE
    cp compile_commands.json ../ && cd ..
    
    
'

# Fix ownership of all generated files
sudo chown -R $USER_ID:$GROUP_ID /sep/.cache /sep/.codechecker /sep/build /sep/output 
fix_compile_commands() {
    # Replace container paths with host paths for IDE integration
    sed -i "s|/sep/|$(pwd)/|g" compile_commands.json
}

# Extract errors from build log
if [ -f output/build_log.txt ]; then
    echo "Extracting errors to output/errors.txt..."
    grep -i "error\|failed\|fatal" output/build_log.txt > output/errors.txt 2>/dev/null || echo "No errors found" > output/errors.txt
fi

# Run include analysis if there were errors
if [ -f output/errors.txt ] && [ -s output/errors.txt ] && [ "$(head -1 output/errors.txt)" != "No errors found" ]; then
    echo "Running include dependency analysis..."
    ./scripts/analyze_includes.sh
fi

echo "Build complete!"
