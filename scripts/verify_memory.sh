#!/usr/bin/env bash
set -euo pipefail

# Install minimal dependencies and run memory manager tests.
REPO_ROOT="$(cd "$(dirname "$0")/.." && pwd)"
cd "$REPO_ROOT"

# Ensure a C++ compiler is specified for dependency scripts
export CXX=${CXX:-$(command -v g++ || true)}

# Install dependencies without CUDA or Cycles for fast test builds.
python3 install_dependencies.py --no-cuda --no-cycles

# Build and run memory_manager_tests using the minimal configuration.
./build_no_cuda.sh
