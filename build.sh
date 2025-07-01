#!/bin/bash
set -eo pipefail
clear

# Source user-provided library paths if available
if [ -f "scripts/local_env_paths.sh" ]; then
  echo "Loading local library paths from scripts/local_env_paths.sh"
  source scripts/local_env_paths.sh
fi

echo "==== SEP Engine Build Script with FULL Cycles and PipeWire Support ===="
echo "Setting up build environment with real library paths - NO STUBS!"

# --- Environment Setup ---
# Determine repository root dynamically so the script works regardless
# of where the project directory is located on disk.
REPO_ROOT="$(cd "$(dirname "$0")" && pwd)"
BUILD_DIR="${REPO_ROOT}/cmake-make"
SRC_DIR="${REPO_ROOT}"
LIB_DIR="${SRC_DIR}/lib"
CYCLES_ROOT_DIR="${SRC_DIR}/extern/cycles"

# Create build directory
mkdir -p "${BUILD_DIR}"

# Create symbolic link for Cycles if it exists in the primary location
if [ ! -L "${CYCLES_ROOT_DIR}" ] && [ -d "${SRC_DIR}/cycles" ]; then
  echo "Found Cycles at ${SRC_DIR}/cycles, creating symlink..."
  mkdir -p "$(dirname "${CYCLES_ROOT_DIR}")"
  ln -sf "${SRC_DIR}/cycles" "${CYCLES_ROOT_DIR}"
fi

if [ ! -d "${CYCLES_ROOT_DIR}" ]; then
  echo "Error: Cycles directory not found at ${SRC_DIR}/cycles or ${CYCLES_ROOT_DIR}."
  exit 1
fi
echo "Using Cycles root: ${CYCLES_ROOT_DIR}"

# --- Dependency Detection ---
# Check for PipeWire using pkg-config (most reliable method)
echo "Checking for PipeWire development headers..."
export PKG_CONFIG_PATH=/usr/lib64/pkgconfig:$PKG_CONFIG_PATH
PIPEWIRE_CMAKE_ARGS=""
if command -v pkg-config >/dev/null && pkg-config --exists libpipewire-0.3; then
  # Rely on pkg-config to locate PipeWire; avoid manually setting unused variables
  echo "PipeWire found via pkg-config"
  # PIPEWIRE_CMAKE_ARGS can remain empty because the CMake scripts handle
  # discovery internally via pkg-config.
else
  echo "PipeWire not found via pkg-config. Audio capture will be disabled."
fi

# --- CMake Configuration ---
# Clean build directory to ensure a fresh state
echo "Cleaning build directory..."
rm -rf "${BUILD_DIR:?}"/*
# Copy cmake modules needed by the project
cp -r "${SRC_DIR}/cmake" "${BUILD_DIR}"

# Setup CMAKE_PREFIX_PATH to help find system libraries
# This is crucial for dependencies like OpenVDB, Alembic, TBB, etc.
CMAKE_PREFIX_PATH="/usr;/usr/local;/opt/local;/opt/homebrew;${LIB_DIR}"

echo "Running CMake configuration..."
cd "${BUILD_DIR}"

cmake -S "${SRC_DIR}" -B "${BUILD_DIR}" \
  -DCMAKE_C_COMPILER=/usr/bin/clang-15 \
  -DCMAKE_CXX_COMPILER=/usr/bin/clang++-15 \
  -DCMAKE_PREFIX_PATH="${CMAKE_PREFIX_PATH}" \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
  -DCMAKE_EXE_LINKER_FLAGS="-Wl,--no-as-needed" \
  ${PIPEWIRE_CMAKE_ARGS}

# --- Build and Install ---
echo "Building SEP Engine..."
# Use all available processor cores for a faster build
make -j$(nproc)

echo "Installing SEP Engine binary..."
# Example of systemd service management. Use 'sudo' only if necessary.
if command -v systemctl >/dev/null; then
    sudo systemctl stop sep-engine || true # Stop if running
    sudo ln -sf "${BUILD_DIR}/sep_engine" /usr/local/bin/sep_engine
    sudo systemctl daemon-reload
    sudo systemctl start sep-engine
    sudo systemctl status sep-engine --no-pager
else
    echo "systemctl not found. Manual installation required for sep_engine binary."
    echo "Binary is at: ${BUILD_DIR}/sep_engine"
fi

echo "==== Build Complete ===="
if [ -f "${BUILD_DIR}/sep_engine" ]; then
    echo "SEP Engine binary is located at: ${BUILD_DIR}/sep_engine"
    echo "Cycles test executable is at: ${BUILD_DIR}/src/tests/cycles_test"
else
    echo "Build failed. Please check the output above for errors."
    exit 1
fi