#!/bin/bash
set -e

ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
BUILD_DIR="$ROOT_DIR/examples/build"

mkdir -p "$BUILD_DIR"

echo "Building drug_optimizer..."

# Compile
g++ -std=c++17 -o "$BUILD_DIR/drug_optimizer" \
    "$ROOT_DIR/examples/drug_optimizer.cpp" \
    -I"$ROOT_DIR/include" \
    -I"$ROOT_DIR/extern/glm" \
    -I"$ROOT_DIR/extern/nlohmann" \
    -I"$ROOT_DIR/examples" \
    -Wl,--start-group \
    "$ROOT_DIR/src/quantum/libsep_quantum.a" \
    "$ROOT_DIR/src/memory/libsep_memory.a" \
    "$ROOT_DIR/src/compat/libsep_compat.a" \
    "$ROOT_DIR/src/core/libsep_core.a" \
    -Wl,--end-group \
    -pthread

echo "Build complete. Executable is at $BUILD_DIR/drug_optimizer"
