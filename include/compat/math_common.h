#pragma once

#ifndef SEP_MATH_COMMON_H
#define SEP_MATH_COMMON_H

#include "compat/shim.h"
#include "compat/macros.h"

/* Standard includes */
#ifdef __cplusplus
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <limits>

// First include C++ std headers for GCC 14.2.1 compatibility
#if defined(__GNUC__) && __GNUC__ >= 14
#ifndef SEP_HAD_CUDACC
#ifdef __CUDACC__
#define SEP_HAD_CUDACC 1
#undef __CUDACC__
#else
#define SEP_HAD_CUDACC 0
#endif
#endif

// Include C++ headers first to avoid conflicts with CUDA headers
#include <cmath>
#include <type_traits>

// Restore __CUDACC__ if it was defined
#if SEP_HAD_CUDACC
#define __CUDACC__ 1
#endif
#undef SEP_HAD_CUDACC
#endif

// GLM compatibility defines - must come before any GLM includes
#define GLM_FORCE_PURE   // Ensure GLM doesn't use any platform-specific optimizations
#define GLM_FORCE_CXX14  // Force C++14 mode for GLM
#ifdef GLM_COMPILER
#undef GLM_COMPILER
#endif
#define GLM_COMPILER 0

#ifdef __CUDACC__
#define CUDA_VERSION 9000  // Force CUDA version to 9.0 for GLM
#define GLM_FORCE_CUDA     // Force CUDA mode

// Use the standard CUDA runtime instead of the custom wrapper
#include <cuda_runtime.h>
#endif

// Include GLM after defining compatibility macros
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#ifndef __CUDACC__
#if !defined(__GNUC__) || __GNUC__ < 14
#include <cmath>
#endif
#endif
#endif

namespace sep {
namespace pattern {
static constexpr float MIN_COHERENCE = 0.1f;
static constexpr float COLLAPSE_THRESHOLD = 0.3f;
static constexpr float MTM_COHERENCE_THRESHOLD = 0.7f;
static constexpr float LTM_COHERENCE_THRESHOLD = 0.9f;
static constexpr float STABILITY_THRESHOLD = 0.85f;
static constexpr float DEMOTION_THRESHOLD = 0.3f;
}  // namespace pattern

namespace cuda {

// Math utility functions
static inline float clamp_f(float value, float min_val, float max_val) {
    return std::min(std::max(value, min_val), max_val);
}

static inline float lerp_f(float a, float b, float t) {
    return a + t * (b - a);
}

static inline float abs_f(float x) {
    return std::abs(x);
}

static inline float sqrt_approx(float x) {
    return std::sqrt(x);
}

static inline float sqrt_safe_f(float value) {
    return value > 0.0f ? std::sqrt(value) : 0.0f;
}

static inline double sqrt_safe_d(double value) {
    return value > 0.0 ? std::sqrt(value) : 0.0;
}

static inline float exp_safe_f(float value) {
    return std::exp(value);
}

static inline double exp_safe_d(double value) {
    return std::exp(value);
}

static inline float log2_safe_f(float value) {
    return value > 0.0f ? std::log2(value) : 0.0f;
}

static inline double log2_safe_d(double value) {
    return value > 0.0 ? std::log2(value) : 0.0;
}

static inline float dot_product(const float* a, const float* b, size_t size) {
    float sum = 0.0f;
    for (size_t i = 0; i < size; ++i) {
        sum += a[i] * b[i];
    }
    return sum;
}

static inline float magnitude_squared(const float* v, size_t size) {
    float sum = 0.0f;
    for (size_t i = 0; i < size; ++i) {
        sum += v[i] * v[i];
    }
    return sum;
}

static inline float magnitude(const float* v, size_t size) {
    return sqrt_safe_f(magnitude_squared(v, size));
}

static inline float calculateCoherence(float stability, float symmetry, float entropy) {
    return clamp_f((stability + symmetry) * (1.0f - entropy), 0.0f, 1.0f);
}

static inline int isCoherenceValid(float coherence) {
    return coherence >= pattern::MIN_COHERENCE && coherence <= 1.0f;
}

static inline float calculateRelationshipStrength(const float* pattern_a, const float* pattern_b, size_t size) {
    float dot = dot_product(pattern_a, pattern_b, size);
    float mag_a = magnitude(pattern_a, size);
    float mag_b = magnitude(pattern_b, size);

    if (mag_a < 1e-6f || mag_b < 1e-6f)
        return 0.0f;

    return clamp_f(dot / (mag_a * mag_b), 0.0f, 1.0f);
}

static inline float calculateQuantumAmplitude(float coherence, float stability) {
    return clamp_f(coherence * stability, 0.0f, 1.0f);
}

static inline int shouldCollapsePattern(float coherence, float collapse_threshold) {
    return coherence < collapse_threshold ? 1 : 0;
}

static inline int shouldCollapsePatternDefault(float coherence) {
    return shouldCollapsePattern(coherence, pattern::COLLAPSE_THRESHOLD);
}

static inline float calculateStability(float symmetry, float coherence_history, float entropy) {
    return clamp_f((symmetry + coherence_history) * (1.0f - entropy), 0.0f, 1.0f);
}

static inline int isPatternViable(float coherence, float min_coherence) {
    return coherence >= min_coherence ? 1 : 0;
}

static inline int isPatternViableDefault(float coherence) {
    return isPatternViable(coherence, pattern::MIN_COHERENCE);
}

static inline int shouldPromoteToMTM(float coherence) {
    return (coherence >= pattern::MTM_COHERENCE_THRESHOLD) ? 1 : 0;
}

static inline int shouldPromoteToLTM(float coherence, float stability, int generations) {
    return (coherence >= pattern::LTM_COHERENCE_THRESHOLD && stability >= pattern::STABILITY_THRESHOLD &&
            generations >= 100)
               ? 1
               : 0;
}

static inline int shouldDemotePattern(float coherence) {
    return (coherence < pattern::DEMOTION_THRESHOLD) ? 1 : 0;
}

}  // namespace cuda

namespace math {
using ::sep::cuda::sqrt_safe_d;
using ::sep::cuda::sqrt_safe_f;
inline float sqrt_safe(float v) {
    return sqrt_safe_f(v);
}
inline double sqrt_safe(double v) {
    return sqrt_safe_d(v);
}
using ::sep::cuda::exp_safe_d;
using ::sep::cuda::exp_safe_f;
inline float exp_safe(float v) {
    return exp_safe_f(v);
}
inline double exp_safe(double v) {
    return exp_safe_d(v);
}
inline float to_float(double v) {
    return static_cast<float>(v);
}
}  // namespace math
}  // namespace sep

#endif  // SEP_MATH_COMMON_H
