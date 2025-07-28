#!/usr/bin/env bash
# SEP Engine dependency installer
set -uo pipefail
sudo ln -sf /workspace/sep /sep
cd /sep


# Pinned Python version used for all installs  
PYTHON_VERSION="3.13.*"

wget https://developer.download.nvidia.com/compute/cuda/repos/ubuntu2404/x86_64/cuda-keyring_1.1-1_all.deb
sudo dpkg -i cuda-keyring_1.1-1_all.deb
sudo apt-get update

# Optional argument parsing
USE_CUDA=1
USE_MINIMAL=0
for arg in "$@"; do
  case "$arg" in
    --no-cuda)
      USE_CUDA=0
      shift
      ;;
    --minimal)
      USE_MINIMAL=1
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

MIN_PACKAGES=(
  build-essential cmake git clang-15 clang-tidy-15 clang-format-15 ninja-build
  libspdlog-dev libfmt-dev libbenchmark-dev libgtest-dev 
  nlohmann-json3-dev pkg-config libhiredis-dev libglm-dev
  libyaml-cpp-dev libimgui-dev libgl1-mesa-dev libglfw3-dev
  libcurl4-openssl-dev curl python3 python3-pip gdb
  libpipewire-0.3-dev libspa-0.2-dev libtbb-dev
  valgrind nodejs npm
)

FULL_PACKAGES=(
  "${MIN_PACKAGES[@]}"
  libglu1-mesa-dev libpcre3-dev libxrandr-dev 
  libboost-all-dev libpugixml-dev libopenjp2-7-dev
  libhttp-parser-dev liblz4-dev libzstd-dev
)

if [ "$USE_MINIMAL" -eq 1 ]; then
  PACKAGES=("${MIN_PACKAGES[@]}")
else
  PACKAGES=("${FULL_PACKAGES[@]}")
fi

echo "Updating package lists..."
sudo apt-get update -y

echo "Installing base packages..."
sudo apt-get install -y "${PACKAGES[@]}" | tee "$LOG_DIR/apt.log"

# Install Docker and Docker Compose
echo "Installing Docker..."
sudo apt-get install -y docker.io docker-compose-v2 >> "$LOG_DIR/apt.log"
sudo systemctl enable --now docker >/dev/null 2>&1 || true
if [ "$EUID" -ne 0 ]; then
  sudo usermod -aG docker "$USER" || true
fi

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
  # Explicitly install the pinned version
  sudo apt-get install -y "python3.13=${PYTHON_VERSION}" \
    "python3.13-dev=${PYTHON_VERSION}" | tee -a "$LOG_DIR/apt.log"
fi

# Install Python packages for analysis
pip3 install pandas numpy matplotlib codechecker

# Set up clang tool symlinks
sudo ln -sf /usr/bin/clang-tidy-15 /usr/bin/clang-tidy
sudo ln -sf /usr/bin/clang-format-15 /usr/bin/clang-format

# Verify installed packages
echo "Verifying installations..."
docker --version || { echo "Docker not installed"; exit 1; }
docker compose version || true
python3.13 --version || true
for pkg in "${PACKAGES[@]}" docker.io docker-compose-v2; do
  if dpkg -s "$pkg" >/dev/null 2>&1; then
    echo "$pkg installed"
  else
    echo "$pkg missing" >&2
  fi
done

