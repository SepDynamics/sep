#!/usr/bin/env bash
set -e

# Unless overridden, install the full dependency set.
: "${INSTALL_MINIMAL:=0}"
: "${INSTALL_CUDA:=0}"

# Base packages required for building and running unit tests
BASE_PACKAGES=(
    build-essential cmake git
    libspdlog-dev libfmt-dev libglm-dev
    libboost-all-dev libasio-dev libssl-dev
    libcurl4-openssl-dev libhttp-parser-dev
    liblz4-dev libzstd-dev
    libgflags-dev libgoogle-glog-dev
    libpugixml-dev libopenjp2-7-dev
    libimath-dev libtbb-dev libopenexr-dev
    libpipewire-0.3-dev libbenchmark-dev
    libgtest-dev libomp-dev
    nlohmann-json3-dev pkg-config
)

# Additional heavy libraries used by rendering integrations
FULL_PACKAGES=(
    libembree-dev libopenvdb-dev
    libopencolorio-dev libopenimageio-dev
)

# Choose package list based on INSTALL_MINIMAL flag
if [ "${INSTALL_MINIMAL}" = "1" ]; then
    PACKAGES=("${BASE_PACKAGES[@]}")
else
    PACKAGES=("${BASE_PACKAGES[@]}" "${FULL_PACKAGES[@]}")
fi

sudo apt-get update
sudo apt-get install -y "${PACKAGES[@]}"

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
