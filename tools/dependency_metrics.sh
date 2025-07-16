#!/bin/bash

# tools/dependency_metrics.sh - Summarize include statistics

set -e

SRC_DIR="$(git rev-parse --show-toplevel)/src"

echo "=== Include Statistics ==="
find "$SRC_DIR" -name "*.h" -o -name "*.hpp" | xargs grep -h "^#include" | wc -l

echo
echo "=== Most Included Files ==="
find "$SRC_DIR" -name "*.h" -o -name "*.hpp" | xargs grep -h "^#include" | \
  sed 's/#include.*[<"]\(.*\)[>"].*/\1/' | sort | uniq -c | sort -nr | head -20

echo
echo "=== Files With Most Includes ==="
for file in $(find "$SRC_DIR" -name "*.h" -o -name "*.hpp"); do
    count=$(grep -c "^#include" "$file" 2>/dev/null || echo 0)
    echo "$count $file"
done | sort -nr | head -20

