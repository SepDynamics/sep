#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
ROOT_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"

# Use minimal dependencies by default; allow override via environment
: "${INSTALL_MINIMAL:=1}"
export INSTALL_MINIMAL

# Ensure a C++ compiler is set for install_dependencies.sh
export CXX="${CXX:-g++}"

"$SCRIPT_DIR/install_dependencies.sh"

BUILD_DIR="$ROOT_DIR/_sep/testbed/memory_minimal/build"
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"
cmake ..
cmake --build . -j "$(nproc)"

./memory_minimal_tests
