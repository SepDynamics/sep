#!/usr/bin/env bash
set -e

# Update package lists
sudo apt-get update

# Install system packages
sudo apt-get install -y \
    build-essential \
    cmake \
    libspdlog-dev \
    libfmt-dev \
    libglm-dev \
    libasio-dev \
    libcurl4-openssl-dev \
    liblz4-dev \
    libzstd-dev \
    libpipewire-0.3-dev \
    libbenchmark-dev \
    libgtest-dev \
    libomp-dev

# Optional: CUDA toolkit for GPU builds
if [ "${INSTALL_CUDA:-}" = "1" ]; then
    wget -q https://developer.download.nvidia.com/compute/cuda/repos/ubuntu2404/x86_64/cuda-keyring_1.1-1_all.deb -O /tmp/cuda-keyring.deb
    sudo dpkg -i /tmp/cuda-keyring.deb
    sudo apt-get update
    sudo apt-get -y install cuda-toolkit-12-9
fi

# Clone Crow (header-only web framework) if not present
if [ ! -d "third_party/crow" ]; then
    git clone https://github.com/CrowCpp/crow.git third_party/crow
fi

# Build GoogleTest libraries
cd /usr/src/googletest
sudo cmake . -B build
sudo cmake --build build --target install
