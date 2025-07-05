#!/bin/bash

set -e

echo "Building drug optimizer..."

mkdir -p /sep/examples/build

g++ -std=c++17 -o /sep/examples/build/drug_optimizer \
    /sep/examples/drug_optimizer.cpp \
    -I/sep/include \
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

echo "Build complete. Executable is at /sep/examples/build/drug_optimizer"

echo "Run with: /sep/examples/build/drug_optimizer"
