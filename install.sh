#!/usr/bin/env bash
# SEP Engine dependency installer
set -euo pipefail

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
)

echo "Updating package lists..."
sudo apt-get update -y

echo "Installing base packages..."
sudo apt-get install -y "${PACKAGES[@]}" | tee "$LOG_DIR/apt.log"

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

build_opensubdiv() {
  local src="$BUILD_DIR/opensubdiv"
  git clone --depth 1 https://github.com/PixarAnimationStudios/OpenSubdiv.git "$src"
  cmake -S "$src" -B "$src/build" -DNO_EXAMPLES=ON -DNO_TUTORIALS=ON -DNO_REGRESSION=ON \
    >"$LOG_DIR/opensubdiv.log" 2>&1
  cmake --build "$src/build" -j"$(nproc)" >>"$LOG_DIR/opensubdiv.log" 2>&1
  sudo cmake --install "$src/build" >>"$LOG_DIR/opensubdiv.log" 2>&1
}

build_usd() {
  local src="$BUILD_DIR/usd"
  git clone --depth 1 https://github.com/PixarAnimationStudios/USD.git "$src"
  cmake -S "$src" -B "$src/build" -DPXR_BUILD_TESTS=OFF -DPXR_BUILD_EXAMPLES=OFF \
    >"$LOG_DIR/usd.log" 2>&1
  cmake --build "$src/build" -j"$(nproc)" >>"$LOG_DIR/usd.log" 2>&1
  sudo cmake --install "$src/build" >>"$LOG_DIR/usd.log" 2>&1
}

if ! dpkg -s libopensubdiv-dev >/dev/null 2>&1; then
  echo "libopensubdiv-dev not found, building from source..."
  build_opensubdiv
  sudo ldconfig
fi

if ! dpkg -s libusd-dev >/dev/null 2>&1; then
  echo "libusd-dev not found, building from source..."
  build_usd
  sudo ldconfig
fi

echo "All dependencies installed. See $LOG_DIR for logs."
