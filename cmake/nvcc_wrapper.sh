#!/bin/bash
#
# This script is a wrapper for nvcc that calls clang++-15 with specific flags
# to work around compatibility issues between CUDA and modern C++ headers.
#

# The actual host compiler
HOST_COMPILER="/usr/bin/clang++-15"

# Filter out problematic flags and add necessary ones
args=()
for arg in "$@"; do
  case "$arg" in
    # Add more flags to filter here if needed
    *)
      args+=("$arg")
      ;;
  esac
done

# Add the flag to solve the noexcept issue
args+=("-Dnoexcept(x)=")

# Call the actual host compiler with the modified arguments
exec "$HOST_COMPILER" "${args[@]}"