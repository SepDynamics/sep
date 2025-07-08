#!/usr/bin/env bash
set -euo pipefail

# Default to skipping CUDA unless INSTALL_CUDA is explicitly set
: "${INSTALL_CUDA:=0}"

# Allow running as root without sudo
[ "$(id -u)" -eq 0 ] && SUDO="" || SUDO="sudo"

# Basic compiler commands for later verification
CXX=${CXX:-$(command -v g++ || echo g++)}

# Update package lists
$SUDO apt-get update -y

# Install system packages (duplicates removed)
PACKAGES=(
    build-essential
    cmake
    libspdlog-dev
    libfmt-dev
    libglm-dev
    libboost-all-dev
    libasio-dev
    libssl-dev
    libcurl4-openssl-dev
    libhttp-parser-dev
    liblz4-dev
    libzstd-dev
    libgflags-dev
    libgoogle-glog-dev
    libembree-dev
    libpugixml-dev
    libopenjp2-7-dev
    libopenvdb-dev
    libimath-dev
    libtbb-dev
    libopenexr-dev
    libopencolorio-dev
    libopenimageio-dev
    libpipewire-0.3-dev
    libbenchmark-dev
    libgtest-dev
    libomp-dev
    nlohmann-json3-dev
    pkg-config
)

$SUDO apt-get install -y "${PACKAGES[@]}"

# Optional: CUDA toolkit for GPU builds
if [ "${INSTALL_CUDA:-}" = "1" ]; then
    wget -q https://developer.download.nvidia.com/compute/cuda/repos/ubuntu2404/x86_64/cuda-keyring_1.1-1_all.deb -O /tmp/cuda-keyring.deb
    $SUDO dpkg -i /tmp/cuda-keyring.deb
    $SUDO apt-get update -y
    $SUDO apt-get -y install cuda-toolkit-12-9
fi

# Clone Crow (header-only web framework) if not present
if [ ! -d "third_party/crow" ]; then
    git clone https://github.com/CrowCpp/crow.git third_party/crow
fi

# Clone GLM (header-only math library) if not present
if [ ! -d "third_party/glm" ]; then
    git clone https://github.com/g-truc/glm.git third_party/glm
fi

# Build GoogleTest libraries
if [ -d /usr/src/googletest ]; then
    cd /usr/src/googletest
    $SUDO cmake . -B build
    $SUDO cmake --build build --target install
    $SUDO ldconfig
fi

# Verify toolchain presence
$CXX --version 2>/dev/null || true
cmake --version
