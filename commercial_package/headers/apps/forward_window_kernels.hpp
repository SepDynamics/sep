#pragma once

#include <vector>
#include <cstdint>
#include <cstddef>

namespace sep::apps::cuda {

struct ForwardWindowResult {
    float coherence = 0.0f;
    float stability = 0.0f;
    float entropy = 0.0f;
    int rupture_count = 0;
    int flip_count = 0;
    float confidence = 0.0f;
};

ForwardWindowResult simulateForwardWindowMetrics(const std::vector<uint8_t>& bits, size_t index_start);

}
