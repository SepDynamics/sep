#!/usr/bin/env bash
# SEP Engine dependency installer
set -euo pipefail

# Optional argument parsing
USE_CUDA=1
for arg in "$@"; do
  case "$arg" in
    --no-cuda)
      USE_CUDA=0
      shift
      ;;
  esac
done

if [ "$USE_CUDA" -eq 0 ]; then
  echo "CUDA support disabled via --no-cuda"
  export SEP_HAS_CUDA=0
else
  export SEP_HAS_CUDA=1
fi

WS_DIR="$(cd "$(dirname "$0")" && pwd)"
LOG_DIR="$WS_DIR/logs"
BUILD_DIR="$WS_DIR/build/deps"
mkdir -p "$LOG_DIR" "$BUILD_DIR"

PACKAGES=(
  build-essential cmake git
  libglu1-mesa-dev libpcre3-dev libtbb-dev libxrandr-dev libglfw3-dev
  libboost-all-dev libopencolorio-dev libopenimageio-dev
  libembree-dev libpugixml-dev libopenjp2-7-dev
  libcurl4-openssl-dev libhttp-parser-dev libopenvdb-dev
  libfmt-dev
  libspdlog-dev libgtest-dev libhiredis-dev
  libpipewire-0.3-dev libfftw3-dev libopenexr-dev
  valgrind
)

echo "Updating package lists..."
sudo apt-get update -y

echo "Installing base packages..."
sudo apt-get install -y "${PACKAGES[@]}" | tee "$LOG_DIR/apt.log"

# Build and install GoogleTest as the packaged version only ships sources
if [ -d /usr/src/googletest ]; then
  echo "Building GoogleTest..."
  sudo cmake /usr/src/googletest -B /usr/src/googletest/build \
    >> "$LOG_DIR/gtest.log" 2>&1
  sudo cmake --build /usr/src/googletest/build --target install \
    >> "$LOG_DIR/gtest.log" 2>&1
  sudo ldconfig
fi

# Fetch header-only dependencies if missing
if [ ! -d "third_party/crow" ]; then
  git clone https://github.com/CrowCpp/crow.git third_party/crow
fi
if [ ! -d "third_party/glm" ]; then
  git clone https://github.com/g-truc/glm.git third_party/glm
fi

# Install Python 3.13 from deadsnakes if not present
if ! command -v python3.13 >/dev/null; then
  echo "Installing Python 3.13..."
  sudo add-apt-repository ppa:deadsnakes/ppa -y
  sudo apt-get update -y
  sudo apt-get install -y python3.13 python3.13-dev | tee -a "$LOG_DIR/apt.log"
fi

# Install GCC 14 if available
if apt-cache show gcc-14 >/dev/null 2>&1; then
  sudo apt-get install -y gcc-14 g++-14 | tee -a "$LOG_DIR/apt.log"
  sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-14 100
  sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-14 100
fi
