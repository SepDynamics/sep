#include "pattern_types.h"
#include <cuda_runtime.h>
#include <device_launch_parameters.h>

namespace {
constexpr int BLOCK_SIZE = 256;
constexpr float CUDA_MIN_COHERENCE = 0.1F;
constexpr float QUANTUM_EPSILON = 1e-5F;

__device__ unsigned int get_global_idx() {
    return blockIdx.x * blockDim.x + threadIdx.x;
}

}  // anonymous namespace

namespace sep {
namespace compat {

__device__ float calculateCoherence(const PatternData& pattern) {
    float real_part = pattern.attributes.x;
    float imag_part = pattern.attributes.y;
    float amplitude = pattern.attributes.z;
    float phase = pattern.attributes.w;

    float coherence = amplitude * cosf(phase) * real_part + amplitude * sinf(phase) * imag_part;
    return fmaxf(0.1F, fminf(1.0F, coherence));
}

__device__ float calculateStability(const PatternData& pattern) {
    return pattern.stability;
}

__device__ void evolvePattern(PatternData& pattern, float evolutionRate, float timeDelta) {
    glm::vec4 attrs = pattern.attributes;

    float mutation = evolutionRate * timeDelta * 1e-5F;
    float2 mutationVector = make_float2(cosf(mutation), sinf(mutation));

    float2 newState = make_float2(attrs.x * mutationVector.x - attrs.y * mutationVector.y,
                                  attrs.x * mutationVector.y + attrs.y * mutationVector.x);

    attrs.x = newState.x;
    attrs.y = newState.y;

    float magnitude = sqrtf((newState.x * newState.x) + (newState.y * newState.y));
    if (magnitude > 1e-5F) {
        attrs.x /= magnitude;
        attrs.y /= magnitude;
        attrs.z = magnitude;
    }

    attrs.w = atan2f(newState.y, newState.x);

    pattern.attributes = attrs;
}

__global__ void processPatternKernel(PatternData* patterns, PatternData* results,
                                       size_t patternCount, const PatternData* previousPatterns) {
    const unsigned int idx = get_global_idx();
    if (idx >= patternCount)
        return;

    PatternData pattern = patterns[idx];

    pattern.coherence = calculateCoherence(pattern);

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
