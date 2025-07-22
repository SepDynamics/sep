#!/bin/bash
# Enhanced build script with development tools integration

set -uo pipefail

echo "Building SEP Engine..."

# Clean up with proper permissions
sudo rm -rf .cache .codechecker CMakeCache.txt output CMakeFiles Makefile /sep/.Trash-1000
mkdir -p .cache output .codechecker/{output,reports,html} build
chmod -R 777 .cache .codechecker build output

# Save text output if the command exists
if command -v totxt.save &> /dev/null; then
    totxt.save
fi

# Ensure proper permissions for CodeChecker directories
USER_ID=$(id -u)
GROUP_ID=$(id -g)

# Ensure Docker image is built
DOCKER_BUILDKIT=1 docker build --no-cache -t sep-engine-builder .

# Function to fix paths in compile_commands.json for host IDE
fix_compile_commands() {
    # Replace container paths with host paths for IDE integration
    sed -i "s|/sep/|$(pwd)/|g" compile_commands.json
}

# Build and setup development environment
docker run --gpus all --rm \
    -v $(pwd):/sep \
    -v $(pwd)/.cache:/home/codecheck/.cache \
    -v $(pwd)/.codechecker:/home/codecheck/.codechecker \
    -e USER_ID=$USER_ID \
    -e GROUP_ID=$GROUP_ID \
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
    # Switch to codecheck user while preserving environment
    sudo -E -u codecheck bash -c "
    # Export CUDA environment variables
    export CUDA_HOME=/usr/local/cuda
    export CUDA_TOOLKIT_ROOT_DIR=/usr/local/cuda
    export CUDA_BIN_PATH=/usr/local/cuda/bin
    export CMAKE_CUDA_COMPILER=/usr/local/cuda/bin/nvcc
    export PATH=/usr/local/cuda/bin:${PATH}
    export LD_LIBRARY_PATH=/usr/local/cuda/lib64:${LD_LIBRARY_PATH-}
    
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
        ./scripts/run_codechecker.sh
    fi
    "
'

# Fix paths in compile_commands.json for local IDE integration
fix_compile_commands
echo "Build complete!"
echo "Checking build results..."
# SEP Workbench Runner with OANDA Credentials
# This script sets up the environment and runs the SEP workbench

echo "==================================="
echo "SEP Workbench with OANDA Trading"
echo "==================================="

# Source the OANDA credentials
if [ -f "keys.txt" ]; then
    echo "Loading OANDA credentials..."
    source keys.txt
else
    echo "Error: keys.txt not found!"
    echo "Please create keys.txt with:"
    export OANDA_API_KEY="9a5380d0af7dc6d3cdd0c9b29cc5917a-02ceee9244b286c586239697d1ab8b95a"
    export OANDA_ACCOUNT_ID="101-001-31229774-001"
    exit 1
fi

# Verify credentials are loaded
if [ -z "$OANDA_API_KEY" ] || [ -z "$OANDA_ACCOUNT_ID" ]; then
    echo "Error: OANDA credentials not properly loaded!"
    exit 1
fi

echo "OANDA Account: $OANDA_ACCOUNT_ID"
echo "Using practice/demo server: api-fxpractice.oanda.com"
echo ""

# Check if build directory exists
if [ ! -d "build" ]; then
    echo "Build directory not found. Running build.sh first..."
    if [ -f "build.sh" ]; then
        ./build.sh
    else
        echo "Error: build.sh not found!"
        exit 1
    fi
fi

# Check if workbench executable exists
if [ ! -f "build/sep_workbench" ]; then
    echo "Error: sep_workbench executable not found!"
    echo "Please build the project first with: ./build.sh"
    exit 1
fi

# Run the workbench with OANDA credentials
echo "Starting SEP Workbench..."
echo "Press Ctrl+C to exit"
echo ""

./run_workbench.sh
