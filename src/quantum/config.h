#pragma once

#include <cstdint>
#include <cstddef>

namespace sep {

struct QuantumThresholdConfig {
    float ltm_coherence_threshold{0.9f};
    float mtm_coherence_threshold{0.6f};
    float stability_threshold{0.8f};
};

struct ProcessingConfig {
    size_t max_patterns;
    float mutation_rate;
    float ltm_coherence_threshold;
    float mtm_coherence_threshold;
    float stability_threshold;
    bool enable_cuda;
};

}