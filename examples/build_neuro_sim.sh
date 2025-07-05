#!/bin/bash
# Build script for neuro_sim example
set -e

echo "Building neuro_sim..."
mkdir -p /sep/examples/build

g++ -std=c++17 -o /sep/examples/build/neuro_sim \
    /sep/examples/neuro_sim.cpp \
    -I/sep/include \
    -I/sep/extern/glm \
    -I/sep/extern/cycles/src \
    -DSEP_HAS_CYCLES \
    -Wl,--start-group \
    /sep/src/quantum/libsep_quantum.a \
    /sep/src/core/libsep_core.a \
    /sep/src/compat/libsep_compat.a \
    -Wl,--end-group \
    -pthread

echo "Build complete. Executable is at /sep/examples/build/neuro_sim"
