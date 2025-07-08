#!/usr/bin/env bash
set -e

# Default to skipping CUDA unless INSTALL_CUDA is explicitly set
: "${INSTALL_CUDA:=0}"

# Avoid interactive prompts when installing packages
export DEBIAN_FRONTEND=noninteractive

# Update package lists
sudo apt-get update -y -q

# Install system packages. We loop so the script gracefully handles
# re-runs where some packages may already be installed.
packages=(
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

missing_pkgs=()
for pkg in "${packages[@]}"; do
    if ! dpkg -s "$pkg" >/dev/null 2>&1; then
        missing_pkgs+=("$pkg")
    fi
done

if [ ${#missing_pkgs[@]} -gt 0 ]; then
    sudo apt-get install -y -q "${missing_pkgs[@]}"
fi

# Optional: CUDA toolkit for GPU builds
if [ "${INSTALL_CUDA:-}" = "1" ]; then
    wget -q https://developer.download.nvidia.com/compute/cuda/repos/ubuntu2404/x86_64/cuda-keyring_1.1-1_all.deb -O /tmp/cuda-keyring.deb
    sudo dpkg -i /tmp/cuda-keyring.deb
    sudo apt-get update -y -q
    sudo apt-get install -y -q cuda-toolkit-12-9
fi

# Clone Crow (header-only web framework) if not present
if [ ! -d "third_party/crow" ]; then
    git clone https://github.com/CrowCpp/crow.git third_party/crow
fi

# Clone GLM (header-only math library) if not present
if [ ! -d "third_party/glm" ]; then
    git clone https://github.com/g-truc/glm.git third_party/glm
fi

# Build GoogleTest libraries if the source is available
if [ -d /usr/src/googletest ]; then
    cd /usr/src/googletest
    sudo cmake . -B build
    sudo cmake --build build --target install
    cd - >/dev/null
fi

# Verify toolchain presence
cmake --version
g++ --version
