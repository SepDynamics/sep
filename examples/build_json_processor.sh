#!/bin/bash

# Build script for json_processor
# This script compiles the JSON processor directly without relying on CMake

set -e  # Exit on error

echo "Building JSON processor..."

# Create build directory if it doesn't exist
mkdir -p /sep/examples/build

# Compile the JSON processor
g++ -std=c++17 -o /sep/examples/build/json_processor \
    /sep/examples/json_processor.cpp \
    -I/sep/include \
    -I/sep/extern/nlohmann \
    -I/sep/extern/glm \
    -I/sep/extern/cycles/src \
    -I/sep/examples \
    -DSEP_HAS_CYCLES \
    -Wl,--start-group \
    /sep/src/api/libsep_api.a \
    /sep/src/audio/libsep_audio.a \
    /sep/src/quantum/libsep_quantum.a \
    /sep/src/memory/libsep_memory.a \
    /sep/src/compat/libsep_compat.a \
    /sep/src/core/libsep_core.a \
    -Wl,--end-group \
    -pthread

echo "Build complete. Executable is at /sep/examples/build/json_processor"
echo "Run with: /sep/examples/build/json_processor /sep/examples/train_data_2021_to_2025.json"