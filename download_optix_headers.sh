#!/bin/bash

# Exit on error
set -e

# Create directory for OptiX headers
OPTIX_DIR="/sep/cmake-make/optix_headers"
INCLUDE_DIR="$OPTIX_DIR/include"

echo "Creating OptiX headers directory at $INCLUDE_DIR"
mkdir -p "$INCLUDE_DIR"

# Define the OptiX header files to download
HEADER_FILES=(
  "optix.h"
  "optix_device.h"
  "optix_function_table.h"
  "optix_host.h"
  "optix_stack_size.h"
  "optix_stubs.h"
  "optix_types.h"
)

# Download each header file from the NVIDIA OptiX GitHub repository
for HEADER_FILE in "${HEADER_FILES[@]}"; do
  HEADER_URL="https://raw.githubusercontent.com/NVIDIA/optix-dev/v8.0.0/include/${HEADER_FILE}"
  HEADER_PATH="$INCLUDE_DIR/${HEADER_FILE}"
  
  echo "Downloading $HEADER_FILE from $HEADER_URL"
  curl -s -o "$HEADER_PATH" "$HEADER_URL"
  
  if [ -f "$HEADER_PATH" ]; then
    echo "Successfully downloaded $HEADER_FILE ($(wc -c < "$HEADER_PATH") bytes)"
  else
    echo "Failed to download $HEADER_FILE"
    exit 1
  fi
done

echo "All OptiX headers downloaded successfully to $INCLUDE_DIR"
ls -la "$INCLUDE_DIR"