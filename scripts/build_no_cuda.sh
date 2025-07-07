#!/bin/bash
set -e

# Simple build helper that disables CUDA for debugging
REPO_ROOT="$(cd "$(dirname "$0")/.." && pwd)"
BUILD_DIR="${REPO_ROOT}/cmake-no-cuda"

rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"

cd "$BUILD_DIR"
cmake -S "$REPO_ROOT" -B "$BUILD_DIR" \
  -DCMAKE_C_COMPILER=/usr/bin/gcc-14 \
  -DCMAKE_CXX_COMPILER=/usr/bin/g++-14 \
  -DSEP_HAS_CUDA=0 \
  -DSEP_USE_CUDA=0 \
  -DCMAKE_DISABLE_FIND_PACKAGE_CUDAToolkit=ON \
  -DSEP_WITH_CYCLES=OFF \
  -DSEP_WITH_AUDIO=OFF

cmake --build "$BUILD_DIR" -j$(nproc)

