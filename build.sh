#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "$0")" && pwd)"
BUILD_DIR="$ROOT_DIR/build"

echo "Cleaning build directory..."
rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"

echo "Configuring project..."
cmake -S "$ROOT_DIR" -B "$BUILD_DIR"

echo "Building all targets..."
cmake --build "$BUILD_DIR" -- -j$(nproc)

echo "==== Build Complete ===="
