#!/bin/bash

# Simple clang-tidy script that works with our Docker setup

# Ensure we have compile_commands.json from the build
if [ ! -f "compile_commands.json" ]; then
    echo "compile_commands.json not found. Please run ./build.sh first"
    exit 1
fi

echo "Running clang-tidy analysis..."

# Create output directory
mkdir -p .codechecker/reports .codechecker/html

# Run clang-tidy in Docker container
docker run --rm \
    -v $(pwd):/sep \
    sep-engine-builder bash -c '
        cd /sep
        echo "Found $(wc -l < compile_commands.json) compilation entries"
        
        # Run clang-tidy on a sample of files to avoid too much output
        find src/ -name "*.cpp" | head -10 | while read file; do
            echo "Analyzing: $file"
            clang-tidy "$file" -p . --format-style=file > ".codechecker/reports/$(basename $file).txt" 2>&1 || true
        done
    '

echo "Clang-tidy analysis complete. Check .codechecker/reports/ for individual file reports."
