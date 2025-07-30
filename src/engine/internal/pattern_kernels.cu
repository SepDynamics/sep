#include <cuda_runtime.h>
#include <device_launch_parameters.h>

#include "pattern_types.h"

namespace {

__device__ unsigned int get_global_idx() {
    return blockIdx.x * blockDim.x + threadIdx.x;
}

}  // anonymous namespace

namespace sep {
namespace compat {

__device__ float calculateCoherence(const PatternData& pattern) {
    // Ensure we have at least 4 attributes
    if (pattern.get_size() < 4) return 0.5f;
    
    float real_part = pattern[0];
    float imag_part = pattern[1];
    float amplitude = pattern[2];
    float phase = pattern[3];

    float coherence = amplitude * cosf(phase) * real_part + amplitude * sinf(phase) * imag_part;
    return fmaxf(0.1F, fminf(1.0F, coherence));
}

__device__ float calculateStability(const PatternData& pattern) {
    // Use the 5th attribute as stability, or default to 0.5
    return pattern.get_size() > 4 ? pattern[4] : 0.5f;
}

__device__ void evolvePattern(PatternData& pattern, float evolutionRate, float timeDelta) {
    // Ensure we have at least 4 attributes
    if (pattern.get_size() < 4) return;
    
    float mutation = evolutionRate * timeDelta * 1e-5F;
    float2 mutationVector = make_float2(cosf(mutation), sinf(mutation));

    float2 newState = make_float2(pattern[0] * mutationVector.x - pattern[1] * mutationVector.y,
                                  pattern[0] * mutationVector.y + pattern[1] * mutationVector.x);

    pattern[0] = newState.x;
    pattern[1] = newState.y;

    float magnitude = sqrtf((newState.x * newState.x) + (newState.y * newState.y));
    if (magnitude > 1e-5F) {
        pattern[0] /= magnitude;
        pattern[1] /= magnitude;
        pattern[2] = magnitude;
    }

    pattern[3] = atan2f(newState.y, newState.x);
}

__global__ void processPatternKernel(PatternData* patterns, PatternData* results,
                                       size_t patternCount, const PatternData* previousPatterns) {
    const unsigned int idx = get_global_idx();
    if (idx >= patternCount)
        return;

    PatternData pattern = patterns[idx];

    // PatternData doesn't have a coherence member, store in attributes if needed
    float coherence = calculateCoherence(pattern);

    float timeDelta = 0.016F;
    float evolutionRate = 0.05F;

    evolvePattern(pattern, evolutionRate, timeDelta);

    if (previousPatterns != nullptr && idx > 0 && idx < patternCount - 1) {
        PatternData leftPattern = patterns[idx - 1];
        PatternData rightPattern = patterns[idx + 1];

        float interactionStrength = 0.05F;
        if (pattern.get_size() >= 2 && leftPattern.get_size() >= 2 && rightPattern.get_size() >= 2) {
            pattern[0] += interactionStrength * (leftPattern[0] + rightPattern[0]) * 0.5F;
            pattern[1] += interactionStrength * (leftPattern[1] + rightPattern[1]) * 0.5F;
        }
    }

    results[idx] = pattern;
}

extern "C" cudaError_t launchProcessPatternKernel(PatternData* patterns, PatternData* results,
                                                    size_t patternCount, const PatternData* previousPatterns,
                                                    cudaStream_t stream) {
    dim3 blockSize(256);
    dim3 gridSize((patternCount + 256 - 1) / 256);

    processPatternKernel<<<gridSize, blockSize, 0, stream>>>(patterns, results, patternCount, previousPatterns);

    return cudaGetLastError();
}

}
}
