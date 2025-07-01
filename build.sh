#!/bin/bash
set -euo pipefail

BUILD_DIR="cmake-make"

# Clean previous build
rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"

# Configure project
cmake -S . -B "$BUILD_DIR"

# Rebuild all targets
cmake --build "$BUILD_DIR" --target clean
cmake --build "$BUILD_DIR" -j$(nproc)
