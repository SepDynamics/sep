#!/bin/bash
set -euo pipefail

# Simple build script that disables CUDA to aid debugging.
REPO_ROOT="$(cd "$(dirname "$0")" && pwd)"
BUILD_DIR="${REPO_ROOT}/cmake-nocuda"

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

cmake -S "$REPO_ROOT" -B "$BUILD_DIR" \
  -DCMAKE_BUILD_TYPE=Debug \
  -DBUILD_TESTING=ON \
  -DCMAKE_C_COMPILER=/usr/bin/gcc \
  -DCMAKE_CXX_COMPILER=/usr/bin/g++-14 \
  -DSEP_WITH_CYCLES=OFF \
  -DSEP_WITH_AUDIO=OFF \
  -DSEP_WITH_OPENSUBDIV=OFF \
  -DSEP_WITH_OPENPGL=OFF \
  -DSEP_WITH_OCIO=OFF \
  -DSEP_WITH_OPENIMAGEIO=OFF \
  -DSEP_WITH_EMBREE=OFF \
  -DSEP_WITH_OPENVDB=OFF \
  -DSEP_WITH_OSL=OFF \
  -DSEP_WITH_ALEMBIC=OFF \
  -DSEP_MINIMAL=ON \
  -DSEP_HAS_CUDA=0 \
  -DSEP_ENABLE_AUDIO=OFF \
  -DSEP_WORKBENCH_DEMO=OFF \
  -DBUILD_TESTING=ON

make memory_manager_tests -j$(nproc)
