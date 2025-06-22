#pragma once

// CUDA macros and compatibility layer
#include "cuda/macros.h"

#ifdef __CUDACC__
#include <cmath>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#endif

// Standard headers
#include <cstddef>

// Project headers - math first
#include "cuda/math_common.h"


// Other project headers
#include "cuda/constants.h"
#include "quantum/types.h"

#ifndef __CUDACC__
#include "cuda/cuda_impl.h"
#include <cmath>
#include <algorithm>
#endif

namespace sep {
namespace cuda {
namespace detail {

// Forward declaration of the CUDA kernel
#ifdef __CUDACC__
SEP_GLOBAL void process_pattern_kernel(
    pattern::PatternData* pattern,
    pattern::PatternData* result,
    const pattern::PatternConfig* config,
    size_t pattern_count,
    const pattern::PatternData* previous_patterns
);
#else
// CPU fallback implementation
namespace {
void process_pattern_kernel(
    pattern::PatternData* pattern,
    pattern::PatternData* result,
    const pattern::PatternConfig* config,
    size_t pattern_count,
    const pattern::PatternData* previous_patterns
) {}
} // anonymous namespace
#endif

} // namespace detail

// Host-side kernel launcher
SEP_HOST cudaError_t launch_pattern_processing(
    pattern::PatternData* pattern,
    pattern::PatternData* result,
    const pattern::PatternConfig& config,
    size_t pattern_count,
    const pattern::PatternData* previous_patterns = nullptr,
    cudaStream_t stream = nullptr
);

} // namespace cuda
} // namespace sep
