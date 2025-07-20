#!/bin/bash

# Initialize variables
SOURCE_FILE=""
OUTPUT_FILE=""
INCLUDES=()
DEFINES=()
FLAGS=()
DEVICE_ONLY=false

# Parse command-line arguments
while [[ $# -gt 0 ]]; do
    case "$1" in
        --device-c)
            DEVICE_ONLY=true
            shift
            ;;
        -o)
            OUTPUT_FILE="$2"
            shift 2
            ;;
        -I*)
            INCLUDES+=("$1")
            shift
            ;;
        -D*)
            DEFINES+=("$1")
            shift
            ;;
        *.cu) # Source file is recognized by .cu extension
            SOURCE_FILE="$1"
            shift
            ;;
        *) # Any other flags are passed as-is
            FLAGS+=("$1")
            shift
            ;;
    esac
done

# Filter includes and flags if DEVICE_ONLY is true
if [[ "$DEVICE_ONLY" == true ]]; then
    filtered_includes=()
    for inc in "${INCLUDES[@]}"; do
        if [[ "$inc" != *libc++* && "$inc" != *c++/v1* ]]; then
            filtered_includes+=("$inc")
        fi
    done
    INCLUDES=("${filtered_includes[@]}")

    filtered_flags=()
    for flag in "${FLAGS[@]}"; do
        if [[ "$flag" != "-nostdinc++" ]]; then
            filtered_flags+=("$flag")
        fi
    done
    FLAGS=("${filtered_flags[@]}")
fi

# Use nvcc for .cu files, clang++ for others
if [[ "$SOURCE_FILE" == *.cu ]]; then
    NVCC_COMMAND=(
        /usr/local/cuda/bin/nvcc
        -c "$SOURCE_FILE"
        -o "$OUTPUT_FILE"
        "${INCLUDES[@]}"
        "${DEFINES[@]}"
        -std=c++17
        --compiler-bindir=/usr/bin/clang++-15
    )

    if [[ "$DEVICE_ONLY" == true ]]; then
        NVCC_COMMAND+=("--device-c")
    fi

    # Add specific CUDA flags
    NVCC_COMMAND+=(
        -DSEP_CUDACC_DISABLE_EXCEPTION_SPEC_CHECKS=1
        -D__CUDACC__=1
        -D__GLIBC_USE_DEPRECATED_SCANF=0
        -D__USE_GNU=0
        -D__USE_MISC=0
        -D_BITS_MATHCALLS_H=1
        -D__MATH_H=1
        -D__GLIBC_USE_ISOC99=0
        -D__GLIBC_USE_IEC_60559_BFP_EXT=0
        -D__GLIBC_USE_IEC_60559_FUNCS_EXT=0
        -gencode arch=compute_70,code=sm_70
        -gencode arch=compute_75,code=sm_75
        -gencode arch=compute_80,code=sm_80
        -gencode arch=compute_86,code=sm_86
        -gencode arch=compute_89,code=sm_89
        -Wno-deprecated-gpu-targets
        --use_fast_math
        --expt-relaxed-constexpr
        --extended-lambda
        -Xcompiler -fPIC
        -Xcompiler -fno-exceptions
        -Xcompiler -Wno-error
        --diag-suppress 20012
        --diag-suppress 541
        --diag-suppress 177
        "${FLAGS[@]}" # Pass any remaining flags
    )

    "${NVCC_COMMAND[@]}"

else # For C++ files (not .cu)
    /usr/bin/clang++-15 \
        -c "$SOURCE_FILE" \
        -o "$OUTPUT_FILE" \
        "${INCLUDES[@]}" \
        "${DEFINES[@]}" \
        "${FLAGS[@]}" \
        -std=c++17 \
        -fPIC
fi