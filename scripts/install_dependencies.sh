#!/usr/bin/env bash
set -e

# Default to skipping CUDA unless INSTALL_CUDA is explicitly set
: "${INSTALL_CUDA:=0}"
# Allow a reduced package set for headless builds
: "${INSTALL_MINIMAL:=0}"

# Update package lists
sudo apt-get update

# Base packages required for all builds
BASE_PACKAGES=(
    build-essential
    cmake
    libspdlog-dev
    libfmt-dev
    libglm-dev
    libboost-all-dev
    libcurl4-openssl-dev
    libhttp-parser-dev
    liblz4-dev
    libzstd-dev
    libgflags-dev
    libgoogle-glog-dev
    libbenchmark-dev
    libgtest-dev
    libomp-dev
    nlohmann-json3-dev
    pkg-config
)

# Additional libraries needed for full renderer builds
RENDER_PACKAGES=(
    libasio-dev
    libssl-dev
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
)

if [ "${INSTALL_MINIMAL}" = "1" ]; then
    sudo apt-get install -y "${BASE_PACKAGES[@]}"
else
    sudo apt-get install -y "${BASE_PACKAGES[@]}" "${RENDER_PACKAGES[@]}"
fi

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

# Clone GLM (header-only math library) if not present
if [ ! -d "third_party/glm" ]; then
    git clone https://github.com/g-truc/glm.git third_party/glm
fi

# Build GoogleTest libraries
cd /usr/src/googletest
sudo cmake . -B build
sudo cmake --build build --target install

# Verify toolchain presence
cmake --version
g++ --version
