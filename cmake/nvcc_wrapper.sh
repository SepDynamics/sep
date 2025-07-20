#!/bin/bash
#
# This script is a wrapper for nvcc that calls clang++-15 with specific flags
# to work around compatibility issues between CUDA and modern C++ headers.
#

# The actual host compiler
HOST_COMPILER="/usr/bin/clang++-15"

# Call the actual host compiler with all original arguments
exec "$HOST_COMPILER" "$@"