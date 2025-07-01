#include "core/types.h"
#include "compat/cuda_common.h"

#include "compat/cuda_unified_fix.h"
#include "quantum/types.h"
#include "quantum/data.hpp"

#include "compat/cuda_helpers.h"
#ifdef __CUDACC__
#include <device_launch_parameters.h>

namespace sep {
namespace pattern {

using Vec4 = ::sep::pattern::Vec4;

namespace {
constexpr int BLOCK_SIZE = 256;
constexpr float CUDA_MIN_COHERENCE = 0.1F;
constexpr float QUANTUM_EPSILON = 1e-5F;

__device__ SEP_FORCEINLINE unsigned int get_global_idx() {
    return blockIdx.x * blockDim.x + threadIdx.x;
}

// Helper to initialize a CUDA-compatible PatternData structure
__device__ void init_pattern_data(PatternData& pattern) {
    // Initialize CUDA-specific fields
    pattern.id_hash = 0;
    pattern.relationship_count = 0;
    pattern.matched = false;
    pattern.match_start = 0;
    pattern.match_end = 0;
    pattern.capture_count = 0;
    
    // Clear buffers
    for (int i = 0; i < 64; i++) {
        if (i < 64) pattern.name_buffer[i] = 0;
        if (i < 128) pattern.desc_buffer[i] = 0;
        if (i < 128) pattern.matched_text_buffer[i] = 0;
    }
    
    // Clear capture buffers
    for (int i = 0; i < PatternData::MAX_CAPTURES; i++) {
        for (int j = 0; j < 64; j++) {
            pattern.capture_buffers[i][j] = 0;
        }
    }
}
}  // anonymous namespace

__device__ float calculateCoherence(const PatternData& pattern) {
    float real_part = pattern.attributes.x;
    float imag_part = pattern.attributes.y;
    float amplitude = pattern.attributes.z;
    float phase = pattern.attributes.w;

    float coherence = amplitude * cosf(phase) * real_part + amplitude * sinf(phase) * imag_part;
    return fmaxf(CUDA_MIN_COHERENCE, fminf(1.0F, coherence));
}

__device__ float calculateStability(const PatternData& pattern) {
    float energy = pattern.attributes.x * pattern.attributes.x + pattern.attributes.y * pattern.attributes.y;
    float momentum = pattern.attributes.z;
    float angular_momentum = pattern.attributes.w;

    float stability = 1.0F / (1.0F + energy + fabsf(momentum) + fabsf(angular_momentum));
    return fmaxf(0.0F, fminf(1.0F, stability));
}

__device__ void evolvePattern(PatternData& pattern, float evolutionRate, float timeDelta) {
    Vec4 attrs = pattern.attributes;

    float mutation = evolutionRate * timeDelta * QUANTUM_EPSILON;
    float2 mutationVector = make_float2(cosf(mutation), sinf(mutation));

    float2 newState = make_float2(attrs.x * mutationVector.x - attrs.y * mutationVector.y,
                                  attrs.x * mutationVector.y + attrs.y * mutationVector.x);

    attrs.x = newState.x;
    attrs.y = newState.y;

    float magnitude = sqrtf((newState.x * newState.x) + (newState.y * newState.y));
    if (magnitude > QUANTUM_EPSILON) {
        attrs.x /= magnitude;
        attrs.y /= magnitude;
        attrs.z = magnitude;
    }

    attrs.w = atan2f(newState.y, newState.x);

    pattern.attributes = attrs;
    pattern.coherence = calculateCoherence(pattern);
    pattern.stability = calculateStability(pattern);
}

__global__ void processPatternKernel(PatternData* patterns, PatternData* results, const PatternConfig config,
                                      size_t patternCount, const PatternData* previousPatterns) {
    const unsigned int idx = get_global_idx();
    if (idx >= patternCount)
        return;

    // Make a local copy of the pattern data
    PatternData pattern = patterns[idx];
    
    // Initialize any CUDA-specific fields
    init_pattern_data(pattern);

    // Calculate core attributes
    pattern.coherence = calculateCoherence(pattern);
    pattern.stability = calculateStability(pattern);

    if (config.enable_mutations) {
        float timeDelta = 0.016F;
        float evolutionRate = 0.05F;

        evolvePattern(pattern, evolutionRate, timeDelta);

        if (previousPatterns != nullptr && idx > 0 && idx < patternCount - 1) {
            PatternData leftPattern = patterns[idx - 1];
            PatternData rightPattern = patterns[idx + 1];

            float interactionStrength = 0.05F;
            pattern.attributes.x += interactionStrength * (leftPattern.attributes.x + rightPattern.attributes.x) * 0.5F;
            pattern.attributes.y += interactionStrength * (leftPattern.attributes.y + rightPattern.attributes.y) * 0.5F;
        }
    }

    // Copy the processed pattern back to results
    results[idx] = pattern;
}

extern "C" cudaError_t launchProcessPatternKernel(PatternData* patterns, PatternData* results, PatternConfig config,
                                                   size_t patternCount, const PatternData* previousPatterns,
                                                   cudaStream_t stream) {
    dim3 blockSize(constants::get_default_block_size());
    dim3 gridSize((patternCount + BLOCK_SIZE - 1) / BLOCK_SIZE);

    processPatternKernel<<<gridSize, blockSize, 0, stream>>>(patterns, results, config, patternCount, previousPatterns);

    return cudaGetLastError();
}

}  // namespace pattern
}  // namespace sep

#endif  // __CUDACC__
