#!/bin/bash

# Install script for SEP Engine project dependencies
# Enhanced with error checking and logging for build troubleshooting

set -euo pipefail  # Exit on error, undefined var, or pipeline failure

# Directory setup
WORKSPACE_DIR="$(dirname "$(realpath "$0")")"
BUILD_DIR="$WORKSPACE_DIR/build"
LOG_DIR="$WORKSPACE_DIR/logs"
OPEN_SUBDIV_LOG="$LOG_DIR/opensubdiv_build.log"
USD_LOG="$LOG_DIR/usd_build.log"

# Create log directory
mkdir -p "$LOG_DIR"
cd "$WORKSPACE_DIR"

echo "Starting SEP Engine dependency installation..."

# Update package list
sudo apt-get update -y

# List of required packages
REQUIRED_PACKAGES=(
    build-essential cmake git
    libglu1-mesa-dev libpcre3-dev
    libtbb-dev libxrandr-dev libglfw3-dev
    libboost-all-dev
    libopencolorio-dev libopenimageio-dev
    libembree-dev libpugixml-dev libopenjp2-7-dev
    libcurl4-openssl-dev libhttp-parser-dev
    libopenvdb-dev
)

# Install required packages
echo "Installing required packages..."
sudo apt-get install -y "${REQUIRED_PACKAGES[@]}" 2>&1 | tee "$LOG_DIR/packages_install.log"

# Install Python 3.13 from deadsnakes PPA
echo "Installing Python 3.13..."
sudo add-apt-repository ppa:deadsnakes/ppa -y
sudo apt-get update -y
sudo apt-get install -y python3.13 python3.13-dev 2>&1 | tee -a "$LOG_DIR/packages_install.log"

# Check and install GCC-14 if available, otherwise use default
if apt-cache show gcc-14 &>/dev/null; then
    echo "Installing GCC-14..."
    sudo apt-get install -y gcc-14 g++-14
    sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-14 100
    sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-14 100
else
    echo "GCC-14 not found, using default GCC version..."
fi

# Function to check build success
check_build_success() {
    local exit_code=$1
    local log_file=$2
    local lib_path=$3
    local lib_name=$4

    if [ "$exit_code" -ne 0 ]; then
        echo "Error: Failed to build $lib_name. Check $log_file for details."
        exit 1
    fi

    if [ ! -f "$lib_path" ]; then
        echo "Error: $lib_name library not found at $lib_path after build."
        exit 1
    fi

    echo "$lib_name built and installed successfully."
}

# Build OpenSubdiv from source if not found
if ! dpkg -l | grep -q libopensubdiv-dev; then
    echo "libopensubdiv-dev not found, building from source..."
    mkdir -p "$BUILD_DIR/opensubdiv"
    cd "$BUILD_DIR/opensubdiv"
    
    git clone https://github.com/PixarAnimationStudios/OpenSubdiv.git . || {
        echo "Failed to clone OpenSubdiv repository."
        exit 1
    }
    
    mkdir -p build && cd build
    cmake -DNO_EXAMPLES=ON -DNO_TUTORIALS=ON -DNO_REGRESSION=ON \
          -DTBB_DIR=/usr/lib/x86_64-linux-gnu/cmake/TBB \
          .. > "$OPEN_SUBDIV_LOG" 2>&1
    make -j"$(nproc)" >> "$OPEN_SUBDIV_LOG" 2>&1
    sudo bash -c "make install >> '$OPEN_SUBDIV_LOG' 2>&1"
    build_status=$?

    check_build_success $build_status "$OPEN_SUBDIV_LOG" "/usr/local/lib/libosdCPU.so" "OpenSubdiv"
fi

# Build USD from source if not found
if ! dpkg -l | grep -q libusd-dev; then
    echo "libusd-dev not found, building from source..."
    mkdir -p "$BUILD_DIR/usd"
    cd "$BUILD_DIR/usd"
    
    git clone https://github.com/PixarAnimationStudios/USD.git . || {
        echo "Failed to clone USD repository."
        exit 1
    }
    
    mkdir -p build && cd build
    cmake -DPXR_BUILD_TESTS=OFF -DPXR_BUILD_EXAMPLES=OFF \
          -DTBB_DIR=/usr/lib/x86_64-linux-gnu/cmake/TBB \
          .. > "$USD_LOG" 2>&1
    make -j"$(nproc)" >> "$USD_LOG" 2>&1
    sudo bash -c "make install >> '$USD_LOG' 2>&1"
    build_status=$?

    check_build_success $build_status "$USD_LOG" "/usr/local/lib/libusd.so" "USD"
fi

# Update library cache
sudo ldconfig

echo "Dependency installation completed successfully."
echo "Next steps:"
echo "1. Check $LOG_DIR for build logs if issues persist."
echo "2. Navigate to your project build directory and run 'cmake .. && make'."
echo "3. Use Codex to assist with implementation fixes, focusing on test suite and demo workbench."
