#!/bin/bash
set -eo pipefail

# Clear the terminal screen at the start of the script
clear

# Determine repository root dynamically
REPO_ROOT="$(cd "$(dirname "$0")" && pwd)"

# Define the output directory and log file path
OUTPUT_DIR="${REPO_ROOT}/output"
LOG_FILE="${OUTPUT_DIR}/build_log.txt"

# Create the output directory if it doesn't exist
mkdir -p "${OUTPUT_DIR}"

# Clean previous output files in the output directory
rm -rf "${OUTPUT_DIR}/*" || true

# Redirect all output to both console and log file
exec 1> >(tee "${LOG_FILE}") 2>&1

echo "==== SEP Engine Workbench Build Script (Minimal Dependencies) ===="
echo "Setting up build environment for workbench only - NO CYCLES/BLENDER/AUDIO"
echo "Build started at $(date)"

# --- Environment Setup ---
BUILD_DIR="${REPO_ROOT}/cmake-make-workbench"
COMPILE_COMMANDS="${BUILD_DIR}/compile_commands.json"
SRC_DIR="${REPO_ROOT}"
LIB_DIR="${SRC_DIR}/lib"

# Find compilers
C_COMPILER="/usr/bin/gcc"
CXX_COMPILER="/usr/bin/g++"

if [ -x "/usr/bin/gcc-14" ]; then
  C_COMPILER="/usr/bin/gcc-14"
fi
if [ -x "/usr/bin/g++-14" ]; then
  CXX_COMPILER="/usr/bin/g++-14"
fi

# Create build directory
mkdir -p "${BUILD_DIR}"

# Create user-owned build directory
USER_BUILD_DIR="$HOME/.cache/sep/workbench-build"
mkdir -p "$USER_BUILD_DIR"

# Clean old build files with error handling
if [ -d "$USER_BUILD_DIR" ]; then
    find "$USER_BUILD_DIR" -type f -exec rm -f {} + 2>/dev/null || true
    find "$USER_BUILD_DIR" -type d -empty -delete 2>/dev/null || true
fi

# Copy cmake modules needed by the project
cp -r "${SRC_DIR}/cmake" "$USER_BUILD_DIR"

# Update build directory reference
BUILD_DIR="$USER_BUILD_DIR"

# Setup CMAKE_PREFIX_PATH to help find system libraries
CMAKE_PREFIX_PATH="/usr;/usr/local;/opt/local;/opt/homebrew;${LIB_DIR}"

echo "Running CMake configuration..."
cd "${BUILD_DIR}"

# Configure with minimal options - explicitly disable Cycles and Blender
cmake -S "${SRC_DIR}" -B "${BUILD_DIR}" \
  -DCMAKE_C_COMPILER=${C_COMPILER} \
  -DCMAKE_CXX_COMPILER=${CXX_COMPILER} \
  -DCMAKE_PREFIX_PATH="${CMAKE_PREFIX_PATH}" \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
  -DSEP_WITH_CYCLES=OFF \
  -DSEP_WITH_AUDIO=OFF \
  -DSEP_ENABLE_AUDIO=OFF \
  -DSEP_ENABLE_BLENDER=OFF \
  -DSEP_HAS_CYCLES=0

# Link compile_commands.json to the repository root for tool integration
ln -sf "${COMPILE_COMMANDS}" "${REPO_ROOT}/compile_commands.json"

# Build only the workbench target
echo "Building SEP Engine Workbench..."
cd "${BUILD_DIR}"
make -j$(nproc) sep_workbench

echo "Build completed at $(date)"
if [ -f "${BUILD_DIR}/examples/workbench/sep_workbench" ]; then
    echo "Workbench executable built successfully at: ${BUILD_DIR}/examples/workbench/sep_workbench"
else
    echo "Error: Workbench executable not found!"
    exit 1
fi