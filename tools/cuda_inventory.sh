#!/bin/bash
# List all CUDA-related calls in the codebase

echo "=== Direct CUDA Calls ==="
grep -rh "cuda[A-Z][a-zA-Z]*(" src/ --include="*.cpp" --include="*.c" --include="*.h" | \
  sed 's/.*\(cuda[A-Za-z0-9_]*\)(.*/\1/' | sort | uniq -c | sort -nr

echo -e "\n=== SEP Wrapped Calls ==="
grep -rh "SEP_cuda[a-zA-Z]*" src/ --include="*.cpp" --include="*.h" | \
  sed 's/.*\(SEP_cuda[A-Za-z0-9_]*\)(.*/\1/' | sort | uniq -c | sort -nr

echo -e "\n=== Namespace Calls ==="
grep -rh "sep::cuda::[a-zA-Z]*" src/ --include="*.cpp" --include="*.h" | \
  sed 's/.*sep::cuda::\([A-Za-z0-9_]*\)(.*/\1/' | sort | uniq -c | sort -nr
