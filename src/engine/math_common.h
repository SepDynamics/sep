#pragma once

#include <cmath>
#include <complex>

namespace sep::math {

// Common mathematical constants
constexpr float PI = 3.14159265359f;
constexpr float TWO_PI = 2.0f * PI;
constexpr float EPSILON = 1e-6f;

// Common mathematical functions
template<typename T>
inline T clamp(T value, T min_val, T max_val) {
    return std::max(min_val, std::min(value, max_val));
}

template<typename T>
inline T lerp(T a, T b, float t) {
    return a + t * (b - a);
}

inline float phase_wrap(float phase) {
    while (phase > PI) phase -= TWO_PI;
    while (phase < -PI) phase += TWO_PI;
    return phase;
}

} // namespace sep::math
