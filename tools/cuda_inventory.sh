#!/bin/bash

# This script scans the source tree and lists all CUDA API calls used.
# It groups them into three categories: direct CUDA calls, SEP wrapper
# calls, and namespace-based calls.

set -euo pipefail

# change to repo root if run from within subdir
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$REPO_ROOT"

echo "=== Direct CUDA Calls ==="
grep -rh "cuda[A-Z][a-zA-Z]*(" src/ --include="*.cpp" --include="*.h" | \
  sed 's/.*\(cuda[A-Z][a-zA-Z]*\)(.*/\1/' | sort | uniq -c | sort -nr

echo -e "\n=== SEP Wrapped Calls ==="
grep -rh "SEP_cuda[a-zA-Z]*(" src/ --include="*.cpp" --include="*.h" | \
  sed 's/.*\(SEP_cuda[a-zA-Z]*\)(.*/\1/' | sort | uniq -c | sort -nr

echo -e "\n=== Namespace Calls ==="
grep -rh "sep::cuda::[a-zA-Z]*(" src/ --include="*.cpp" --include="*.h" | \
  sed 's/.*sep::cuda::\([a-zA-Z]*\)(.*/\1/' | sort | uniq -c | sort -nr
