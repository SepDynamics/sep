#!/usr/bin/env bash
set -e

# Default to skipping CUDA unless INSTALL_CUDA is explicitly set
: "${INSTALL_CUDA:=0}"

# Detect package manager and appropriate packages
if command -v apt-get >/dev/null; then
    PM_UPDATE="sudo apt-get update"
    PM_INSTALL="sudo apt-get install -y"
    PKGS=(
        build-essential cmake libspdlog-dev libfmt-dev libglm-dev
        libboost-all-dev libasio-dev libssl-dev libcurl4-openssl-dev
        libhttp-parser-dev liblz4-dev libzstd-dev libgflags-dev
        libgoogle-glog-dev libembree-dev libpugixml-dev libopenjp2-7-dev
        libopenvdb-dev libimath-dev libtbb-dev libopenexr-dev
        libopencolorio-dev libopenimageio-dev libpipewire-0.3-dev
        libbenchmark-dev libgtest-dev libomp-dev nlohmann-json3-dev
        pkg-config
    )
elif command -v dnf >/dev/null; then
    PM_UPDATE="sudo dnf -y update"
    PM_INSTALL="sudo dnf install -y"
    PKGS=(
        gcc gcc-c++ make cmake spdlog-devel fmt-devel glm-devel
        boost-devel asio-devel openssl-devel libcurl-devel http-parser-devel
        lz4-devel zstd-devel gflags-devel glog-devel embree-devel pugixml-devel
        openjpeg2-devel openvdb-devel imath-devel tbb-devel OpenEXR-devel
        opencolorio-devel OpenImageIO-devel pipewire-devel benchmark-devel
        gtest-devel libomp-devel nlohmann-json-devel pkgconf-pkg-config
    )
else
    echo "Unsupported package manager. Install dependencies manually." >&2
    exit 1
fi

$PM_UPDATE
$PM_INSTALL "${PKGS[@]}"

# Optional: CUDA toolkit for GPU builds
if [ "${INSTALL_CUDA:-}" = "1" ]; then
    if [ "$PM_INSTALL" = "sudo apt-get install -y" ]; then
        wget -q https://developer.download.nvidia.com/compute/cuda/repos/ubuntu2404/x86_64/cuda-keyring_1.1-1_all.deb -O /tmp/cuda-keyring.deb
        sudo dpkg -i /tmp/cuda-keyring.deb
        sudo apt-get update
        sudo apt-get -y install cuda-toolkit-12-9
    elif [ "$PM_INSTALL" = "sudo dnf install -y" ]; then
        sudo dnf config-manager --add-repo https://developer.download.nvidia.com/compute/cuda/repos/fedora39/x86_64/cuda-fedora39.repo
        sudo dnf install -y cuda-toolkit-12-9
    else
        echo "CUDA installation not supported for this package manager" >&2
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

# Build GoogleTest libraries
cd /usr/src/googletest
sudo cmake . -B build
sudo cmake --build build --target install

# Verify toolchain presence
cmake --version
g++ --version
