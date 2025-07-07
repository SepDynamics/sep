#!/bin/bash
set -euo pipefail

# Simple build script that disables CUDA to aid debugging.
REPO_ROOT="$(cd "$(dirname "$0")" && pwd)"
BUILD_DIR="${REPO_ROOT}/cmake-nocuda"

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

cmake -S "$REPO_ROOT" -B "$BUILD_DIR" \
  -DCMAKE_C_COMPILER=/usr/bin/gcc \
  -DCMAKE_CXX_COMPILER=/usr/bin/g++ \
  -DSEP_WITH_CYCLES=OFF \
  -DSEP_WITH_AUDIO=OFF \
  -DSEP_HAS_CUDA=0 \
  -DSEP_ENABLE_AUDIO=OFF \
  -DSEP_WORKBENCH_DEMO=OFF

make -j$(nproc)
