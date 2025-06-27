#!/bin/bash
set -eo pipefail

echo "==== SEP Engine Build Script with Cycles and PipeWire Support ===="
echo "Setting up build environment..."

# Ensure Cycles directory exists
if [ ! -d "/sep/extern/cycles" ]; then
  echo "Error: Cycles directory not found at /sep/extern/cycles"
  echo "Please clone or copy Cycles repository to this location"
  exit 1
fi

# Run the Cycles environment setup script
if [ -f "/sep/scripts/setup_cycles_env_fixed.sh" ]; then
  echo "Setting up Cycles environment..."
  bash /sep/scripts/setup_cycles_env_fixed.sh
else
  echo "Warning: setup_cycles_env_fixed.sh not found, Cycles setup may be incomplete"
fi

# Ensure PipeWire development headers are installed
if [ -d "/usr/include/pipewire" ] && [ -d "/usr/include/spa" ]; then
  echo "PipeWire development headers found"
else
  echo "Warning: PipeWire development headers not found"
  echo "Audio capture may not work correctly"
fi

# Create build directory
mkdir -p /sep/build

# Configure with CMake, explicitly enabling Cycles
cd /sep/build
echo "Running CMake configuration with Cycles and PipeWire support..."
cmake .. \
  -DSEP_HAS_CYCLES=ON \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

# Build the project
echo "Building SEP Engine..."
make -j$(nproc)

echo "==== Build Complete ===="
echo "If successful, the SEP Engine binary is at: /sep/build/sep_engine"
echo "Run it to verify both Cycles renderer and PipeWire audio are enabled"