#!/bin/bash
# Enhanced build script with development tools integration

set -uo pipefail

echo "Building SEP Engine..."

# Clean up with proper permissions
# sudo rm -rf .cache .codechecker CMakeCache.txt output CMakeFiles Makefile /sep/.Trash-1000
# mkdir -p .cache output .codechecker/{output,reports,html} build
# chmod -R 777 .cache .codechecker build output
cd /sep
sudo rm -rf .cache .codechecker CMakeCache.txt CMakeFiles output Makefile
sleep 2 
clear 
sudo rm -rf /sep/.Trash-1000 
sleep 1

mkdir .cache .codechecker/output output build

# Save text output if the command exists
if command -v totxt.save &> /dev/null; then
    totxt.save
fi

# Ensure proper permissions for CodeChecker directories
USER_ID=$(id -u)
GROUP_ID=$(id -g)

# Ensure Docker image is built
DOCKER_BUILDKIT=1 docker build -t sep-engine-builder .
# DOCKER_BUILDKIT=1 docker build --no-cache -t sep-engine-builder .

# Function to fix paths in compile_commands.json for host IDE
fix_compile_commands() {
    # Replace container paths with host paths for IDE integration
    sed -i "s|/sep/|$(pwd)/|g" compile_commands.json
}

# Build and setup development environment
docker run --gpus all --rm \
    -v $(pwd):/sep \
    -e CUDA_HOME=/usr/local/cuda \
    -e CUDA_TOOLKIT_ROOT_DIR=/usr/local/cuda \
    -e CUDA_BIN_PATH=/usr/local/cuda/bin \
    -e CMAKE_CUDA_COMPILER=/usr/local/cuda/bin/nvcc \
    -e PATH=/usr/local/cuda/bin:${PATH} \
    -e LD_LIBRARY_PATH=/usr/local/cuda/lib64:${LD_LIBRARY_PATH-} \
    sep-engine-builder bash -c '
    # Verify CUDA environment
    echo "Verifying CUDA environment..."
    echo "CUDA_HOME: $CUDA_HOME"
    echo "CUDA_TOOLKIT_ROOT_DIR: $CUDA_TOOLKIT_ROOT_DIR"
    echo "CUDA_BIN_PATH: $CUDA_BIN_PATH"
    echo "CMAKE_CUDA_COMPILER: $CMAKE_CUDA_COMPILER"
    ls -la $CUDA_HOME/bin/nvcc || echo "NVCC not found!"
    # Build as root - no user switching needed
    cd /sep
    rm -rf build
    mkdir -p build
    cd build
    
    # Configure
    cmake .. -G Ninja \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_C_COMPILER=clang-15 \
        -DCMAKE_EXPORT_COMPILE_COMMANDS=TRUE \
        -DCMAKE_CXX_COMPILER=clang++-15 \
        -DSEP_USE_CUDA=ON
    
    # Build
    ninja
    
    # Copy and fix compile_commands.json for IDE
    cp compile_commands.json ../ && cd ..
    
    # Fix ownership of all generated files
    sudo chown -R $USER_ID:$GROUP_ID /sep/.cache /sep/.codechecker /sep/build /sep/output /home/codecheck/.cache /home/codecheck/.codechecker
    
    # Run static analysis if requested
    if [ "${RUN_ANALYSIS:-}" = "true" ]; then
        echo "Running CodeChecker analysis..."
        ./run_codechecker.sh
    fi
'

# Fix paths in compile_commands.json for local IDE integration
fix_compile_commands
echo "Build complete!"
