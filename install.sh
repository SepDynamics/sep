#!/usr/bin/env bash
# SEP Engine dependency installer
set -euo pipefail
export DEBIAN_FRONTEND=noninteractive
SUDO=""
if [ "${EUID:-$(id -u)}" -ne 0 ]; then
  if command -v sudo >/dev/null 2>&1; then
    SUDO="sudo"
  else
    echo "This script requires root privileges or sudo." >&2
    exit 1
  fi
fi

$SUDO ln -sf /workspace/sep /sep
cd /sep

# Reserved for future Python version selection if needed

# Optional argument parsing must occur before any package operations
USE_CUDA=1
USE_MINIMAL=0
USE_LOCAL_CUDA=0
SKIP_DOCKER=0
# Track whether the NVIDIA repository is available
USE_CUDA_REPO=1
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
    --local)
      USE_LOCAL_CUDA=1
      shift
      ;;
    --no-docker)
      SKIP_DOCKER=1
      shift
      ;;
  esac
done

if [ "$USE_CUDA" -eq 1 ] && [ "$USE_LOCAL_CUDA" -eq 0 ]; then
  if wget -q https://developer.download.nvidia.com/compute/cuda/repos/ubuntu2404/x86_64/cuda-keyring_1.1-1_all.deb; then
    $SUDO dpkg -i cuda-keyring_1.1-1_all.deb
    rm cuda-keyring_1.1-1_all.deb
    $SUDO apt-get update
  else
    echo "Warning: Unable to download CUDA keyring, falling back to distro packages"
    USE_CUDA_REPO=0
  fi
elif [ "$USE_CUDA" -eq 1 ] && [ "$USE_LOCAL_CUDA" -eq 1 ]; then
  echo "Using local CUDA installer method"
  # Check for local CUDA installer
  if [ -f "cuda_12.9.0_550.54.15_linux.run" ]; then
    echo "Installing CUDA from local .run file..."
    chmod +x cuda_12.9.0_550.54.15_linux.run
    $SUDO ./cuda_12.9.0_550.54.15_linux.run --silent --toolkit --no-opengl-libs
  else
    echo "Error: Local CUDA installer 'cuda_12.9.0_550.54.15_linux.run' not found"
    echo "Please download it from NVIDIA CUDA Downloads and place it in this directory"
    USE_CUDA=0
  fi
fi

if [ "$USE_CUDA" -eq 0 ]; then
  echo "CUDA support disabled via --no-cuda or missing keyring"
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
$SUDO apt-get update -y
$SUDO dpkg --configure -a >/dev/null 2>&1 || true

# Prevent ca-certificates-java errors on minimal systems
$SUDO mkdir -p /lib/security /etc/ssl/certs/java
$SUDO touch /lib/security/cacerts
$SUDO ln -sf /lib/security/cacerts /etc/ssl/certs/java/cacerts

# Pre-emptively hold ca-certificates-java to prevent installation conflicts
$SUDO apt-mark hold ca-certificates-java >> "$LOG_DIR/apt.log" 2>&1 || true

echo "Installing base packages..."
# Install packages one by one to isolate ca-certificates-java issues
for pkg in "${PACKAGES[@]}"; do
    if [[ "$pkg" == *"java"* ]]; then
        echo "Skipping Java-related package: $pkg"
        continue
    fi
    $SUDO apt-get install -y --no-install-recommends "$pkg" >> "$LOG_DIR/apt.log" 2>&1 || {
        echo "Warning: Failed to install $pkg, continuing..." >&2
    }
done

$SUDO dpkg --configure -a >> "$LOG_DIR/apt.log" 2>&1 || true
$SUDO apt-get purge -y ca-certificates-java >> "$LOG_DIR/apt.log" 2>&1 || true
$SUDO apt-mark unhold ca-certificates-java >> "$LOG_DIR/apt.log" 2>&1 || true

# Install CUDA toolkit when enabled and nvcc missing (only for repo-based installs)
if [ "$USE_CUDA" -eq 1 ] && [ "$USE_LOCAL_CUDA" -eq 0 ]; then
  if ! command -v nvcc >/dev/null 2>&1; then
    echo "Installing CUDA toolkit..."
    if [ "$USE_CUDA_REPO" -eq 1 ]; then
      $SUDO apt-get install -y --no-install-recommends cuda-toolkit-12-9 cuda-nvcc-12-9 >> "$LOG_DIR/apt.log"
    else
      # pre-create java keystore path to avoid post-install errors
      $SUDO mkdir -p /lib/security /etc/ssl/certs/java
      $SUDO touch /lib/security/cacerts
      $SUDO ln -sf /lib/security/cacerts /etc/ssl/certs/java/cacerts
      set +e
      $SUDO apt-get install -y --no-install-recommends nvidia-cuda-toolkit >> "$LOG_DIR/apt.log"
      $SUDO dpkg --configure -a >> "$LOG_DIR/apt.log" 2>&1
      $SUDO apt-get purge -y ca-certificates-java >> "$LOG_DIR/apt.log" 2>&1
      set -e
    fi
    # Create standard CUDA symlink when using distro toolkit
    if command -v nvcc >/dev/null 2>&1 && [ ! -d /usr/local/cuda ]; then
      $SUDO ln -s /usr/lib/nvidia-cuda-toolkit /usr/local/cuda
      export PATH=/usr/local/cuda/bin:$PATH
    fi
  fi
  # Fallback to local installer if nvcc is still missing
  if ! command -v nvcc >/dev/null 2>&1; then
    if [ -f "cuda_12.9.0_550.54.15_linux.run" ]; then
      echo "nvcc not found, using local CUDA installer..."
      chmod +x cuda_12.9.0_550.54.15_linux.run
      $SUDO ./cuda_12.9.0_550.54.15_linux.run --silent --toolkit --no-opengl-libs
    else
      echo "Warning: nvcc still missing and no local installer found" >&2
    fi
  fi
  # Create CUDA compatibility symlinks when using the distro toolkit
  if [ -d /usr/lib/nvidia-cuda-toolkit ]; then
    echo "Creating /usr/local/cuda symlinks for distro toolkit"
    $SUDO ln -sf /usr/lib/nvidia-cuda-toolkit /usr/local/cuda
    $SUDO ln -sf /usr/lib/nvidia-cuda-toolkit/bin /usr/local/cuda/bin
    $SUDO ln -sf /usr/lib/x86_64-linux-gnu /usr/local/cuda/lib64
  fi
fi

# Install Docker and Docker Compose
if [ "$SKIP_DOCKER" -eq 0 ]; then
  echo "Installing Docker..."
  $SUDO apt-get install -y docker.io docker-compose-v2 >> "$LOG_DIR/apt.log"
  $SUDO systemctl enable --now docker >/dev/null 2>&1 || true
  if [ "${EUID:-$(id -u)}" -ne 0 ]; then
    $SUDO usermod -aG docker "$USER" || true
  fi
else
  echo "Skipping Docker installation (--no-docker)"
fi

# Build and install GoogleTest as the packaged version only ships sources
if [ -d /usr/src/googletest ]; then
  echo "Building GoogleTest..."
  $SUDO cmake /usr/src/googletest -B /usr/src/googletest/build \
    >> "$LOG_DIR/gtest.log" 2>&1
  $SUDO cmake --build /usr/src/googletest/build --target install \
    >> "$LOG_DIR/gtest.log" 2>&1
  $SUDO ldconfig
fi

# Fetch header-only dependencies if missing
if [ ! -d "third_party/crow" ]; then
  git clone https://github.com/CrowCpp/crow.git third_party/crow
fi
if [ ! -d "third_party/glm" ]; then
  git clone https://github.com/g-truc/glm.git third_party/glm
fi

# Ensure Python and pip are available
if ! command -v python3 >/dev/null; then
  echo "Installing system Python..."
  $SUDO apt-get install -y python3 python3-dev | tee -a "$LOG_DIR/apt.log"
fi
if ! command -v pip3 >/dev/null; then
  $SUDO apt-get install -y python3-pip | tee -a "$LOG_DIR/apt.log"
fi

# Install Python packages for analysis
# Install Python packages for analysis. Use --break-system-packages to
# allow pip to modify system-managed environments in Ubuntu 24.04.
python3 -m pip install --break-system-packages pandas numpy matplotlib codechecker

# Set up clang tool symlinks
$SUDO ln -sf /usr/bin/clang-tidy-15 /usr/bin/clang-tidy
$SUDO ln -sf /usr/bin/clang-format-15 /usr/bin/clang-format

# Verify installed packages
echo "Verifying installations..."
docker --version || { echo "Docker not installed"; exit 1; }
docker compose version || true
python3 --version || true
if [ "$USE_CUDA" -eq 1 ]; then
  nvcc --version || { echo "NVCC not installed" >&2; exit 1; }
fi
EXTRA_PKGS=()
if [ "$SKIP_DOCKER" -eq 0 ]; then
  EXTRA_PKGS+=(docker.io docker-compose-v2)
fi
for pkg in "${PACKAGES[@]}" "${EXTRA_PKGS[@]}"; do
  if dpkg -s "$pkg" >/dev/null 2>&1; then
    echo "$pkg installed"
  else
    echo "$pkg missing" >&2
  fi
done

# Ensure CUDA in PATH when installed via distro packages
if [ -d /usr/local/cuda/bin ] && ! echo "$PATH" | grep -q "/usr/local/cuda/bin"; then
  export PATH=/usr/local/cuda/bin:$PATH
fi

# Build Docker image used by build.sh if Docker is available
if [ "$SKIP_DOCKER" -eq 0 ]; then
  if $SUDO docker info >/dev/null 2>&1; then
    if ! $SUDO docker image inspect sep-engine-builder >/dev/null 2>&1; then
      echo "Building sep-engine-builder Docker image..."
      $SUDO docker build -t sep-engine-builder .
    fi
  else
    echo "Warning: Docker is not running, skipping image build" >&2
  fi
else
  echo "Skipping Docker image build (--no-docker)"
fi

