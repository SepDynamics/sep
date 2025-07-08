#!/usr/bin/env bash
set -euo pipefail

# Unless overridden, install the full dependency set.
: "${INSTALL_MINIMAL:=0}"
: "${INSTALL_CUDA:=0}"

# Use sudo if not running as root
if [ "$(id -u)" -ne 0 ]; then
    SUDO="sudo"
else
    SUDO=""
fi

# Detect package manager and define install commands
if command -v apt-get >/dev/null 2>&1; then
    PM="apt-get"
    UPDATE_CMD="$SUDO apt-get update -y"
    INSTALL_CMD="$SUDO apt-get install -y"
    BASE_PKGS=(
        build-essential cmake git
        libspdlog-dev libfmt-dev libglm-dev libboost-all-dev
        libasio-dev libssl-dev libcurl4-openssl-dev libhttp-parser-dev
        liblz4-dev libzstd-dev libgflags-dev libgoogle-glog-dev
        libpugixml-dev libopenjp2-7-dev
        libimath-dev libtbb-dev libopenexr-dev
        libpipewire-0.3-dev libbenchmark-dev
        libgtest-dev libomp-dev nlohmann-json3-dev pkg-config
    )
    EXTRA_PKGS=(
        libembree-dev libopenvdb-dev
        libopencolorio-dev libopenimageio-dev
    )
elif command -v dnf >/dev/null 2>&1; then
    PM="dnf"
    UPDATE_CMD="$SUDO dnf -y update"
    INSTALL_CMD="$SUDO dnf -y install"
    BASE_PKGS=(
        gcc gcc-c++ make cmake git
        spdlog-devel fmt-devel glm-devel boost-devel
        asio-devel openssl-devel libcurl-devel http-parser-devel
        lz4-devel zstd-devel gflags-devel glog-devel
        pugixml-devel openjpeg2-devel
        imath-devel tbb-devel openexr-devel
        pipewire-devel benchmark-devel
        gtest-devel libomp-devel nlohmann-json-devel pkgconfig
    )
    EXTRA_PKGS=(
        embree-devel openvdb-devel
        OpenColorIO-devel OpenImageIO-devel
    )
else
    echo "Error: supported package manager not found (apt-get or dnf)" >&2
    exit 1
fi

if [ "$INSTALL_MINIMAL" = "1" ]; then
    PKGS=("${BASE_PKGS[@]}")
else
    PKGS=("${BASE_PKGS[@]}" "${EXTRA_PKGS[@]}")
fi

# Update package lists
echo "Updating package lists using $PM..."
$UPDATE_CMD

# Install system packages
echo "Installing system packages..."
$INSTALL_CMD "${PKGS[@]}"

# Optional: CUDA toolkit for GPU builds
if [ "${INSTALL_CUDA:-}" = "1" ]; then
    if [ "$PM" = "apt-get" ]; then
        wget -q https://developer.download.nvidia.com/compute/cuda/repos/ubuntu2404/x86_64/cuda-keyring_1.1-1_all.deb -O /tmp/cuda-keyring.deb
        $SUDO dpkg -i /tmp/cuda-keyring.deb
        $SUDO apt-get update
        $SUDO apt-get -y install cuda-toolkit-12-9
    elif [ "$PM" = "dnf" ]; then
        wget -q https://developer.download.nvidia.com/compute/cuda/repos/fedora40/x86_64/cuda-fedora40.repo -O /etc/yum.repos.d/cuda.repo
        $SUDO dnf -y clean expire-cache
        $SUDO dnf -y install cuda-toolkit-12-9
    fi
fi

# Clone Crow (header-only web framework) if not present
if [ ! -d "third_party/crow" ]; then
    git clone https://github.com/CrowCpp/crow.git third_party/crow
fi

# Clone GLM (header-only math library) if not present
if [ ! -d "third_party/glm" ]; then
    git clone https://github.com/g-truc/glm.git third_party/glm
fi

# Build GoogleTest libraries if sources are installed
if [ -d /usr/src/googletest ]; then
    cd /usr/src/googletest
    $SUDO cmake . -B build
    $SUDO cmake --build build --target install
fi

# Verify toolchain presence
$CXX --version 2>/dev/null || true
cmake --version
