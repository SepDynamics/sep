#!/usr/bin/env bash
# Build and run memory_manager_tests using the minimal configuration.
set -euo pipefail
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
REPO_ROOT="$(dirname "$SCRIPT_DIR")"

# Ensure dependencies are installed via install.sh
if [ ! -d "$REPO_ROOT/third_party/glm" ]; then
  echo "Dependencies appear missing. Run ./install.sh --minimal first." >&2
  exit 1
fi

# Build in a throwaway directory using the helper script
cd "$REPO_ROOT"
./build_no_cuda.sh
