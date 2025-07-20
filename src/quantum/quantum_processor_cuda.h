#ifndef SEP_QUANTUM_PROCESSOR_CUDA_H
#define SEP_QUANTUM_PROCESSOR_CUDA_H

#include <glm/vec3.hpp>
#include <memory>

#include "quantum/quantum_processor.h"

namespace sep::quantum {

class QuantumProcessorCUDA : public QuantumProcessor {
public:
    explicit QuantumProcessorCUDA(const Config& config);
    ~QuantumProcessorCUDA() override;

    float calculateCoherence(const glm::vec3& a, const glm::vec3& b) override;
    // Other methods will be overridden as we port them to CUDA
};

} // namespace sep::quantum

#endif // SEP_QUANTUM_PROCESSOR_CUDA_H