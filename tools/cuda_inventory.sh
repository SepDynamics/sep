#!/bin/bash

# tools/cuda_inventory.sh - Analyze CUDA API usage
# Lists direct CUDA calls, SEP wrapper calls, and namespace calls

set -e

SRC_DIR="$(git rev-parse --show-toplevel)/src"

echo "=== Direct CUDA Calls ==="
grep -rh "cuda[A-Z][a-zA-Z]*(" "$SRC_DIR" --include="*.cpp" --include="*.h" | \
  sed 's/.*\(cuda[A-Z][a-zA-Z]*\)(.*/\1/' | sort | uniq -c | sort -nr

echo
echo "=== SEP Wrapped Calls ==="
grep -rh "SEP_cuda[a-zA-Z]*(" "$SRC_DIR" --include="*.cpp" --include="*.h" | \
  sed 's/.*\(SEP_cuda[a-zA-Z]*\)(.*/\1/' | sort | uniq -c | sort -nr

echo
echo "=== Namespace Calls ==="
grep -rh "sep::cuda::[a-zA-Z]*(" "$SRC_DIR" --include="*.cpp" --include="*.h" | \
  sed 's/.*sep::cuda::\([a-zA-Z]*\)(.*/\1/' | sort | uniq -c | sort -nr

