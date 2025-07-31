#ifndef SEP_QUANTUM_BITSPACE_FORWARD_WINDOW_RESULT_H
#define SEP_QUANTUM_BITSPACE_FORWARD_WINDOW_RESULT_H

#include <cstdint>

namespace sep::quantum::bitspace {

struct ForwardWindowResult {
    float coherence = 0.0f;
    float stability = 0.0f;
    float entropy = 0.0f;
    int rupture_count = 0;
    int flip_count = 0;
    float confidence = 0.0f;
    // The following fields are for damped results from the kernel
    double damped_coherence = 0.0;
    double damped_stability = 0.0;
};

} // namespace sep::quantum::bitspace

#endif // SEP_QUANTUM_BITSPACE_FORWARD_WINDOW_RESULT_H