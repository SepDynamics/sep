#!/bin/bash
# Simple script to rebuild just the sep executable with static linking

echo "Rebuilding SEP executable with static linking..."

# Run the build in Docker
docker run --gpus all --rm -v $(pwd):/sep sep-engine-builder /bin/bash -c "
    cd /project/build
    # Just rebuild the sep executable
    ninja sep
    # Copy to host
    cp sep /sep/build/
    echo 'Build complete. Checking dependencies:'
    ldd sep || echo 'ldd failed - might be fully static'
"

echo "Checking host dependencies:"
ldd build/sep 2>&1 | grep -E "(not found|=>)" || echo "Executable appears to be statically linked"

echo "Testing executable:"
./build/sep --help 2>&1 | head -20