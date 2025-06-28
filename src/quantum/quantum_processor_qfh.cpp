#include "quantum/quantum_processor_qfh.h"
#include "quantum/types.h"
#include "quantum/processor.h"
#include "compat/component_bridge.h" // For factory functions

namespace sep::quantum {


const QFHResult& QuantumProcessorQFH::getLastQFHResult() const {
    return lastQFHResult();
}

sep::MemoryTierEnum QuantumProcessorQFH::determineMemoryTier(float coherence, float stability,
                                                        uint32_t generation_count) const {
    if (coherence >= 0.9f && stability >= 0.85f && generation_count >= 100)
        return sep::MemoryTierEnum::LTM;
    if (coherence >= 0.7f && generation_count >= 5)
        return sep::MemoryTierEnum::MTM;
    return sep::MemoryTierEnum::STM;
}

// Factory function implementations
std::unique_ptr<IQuantumProcessorQFH> createQuantumProcessorQFH() { // Fix: Add missing definition
    // In a real build, this would check for CUDA availability.
    return std::make_unique<QuantumProcessorQFH>();
}

std::unique_ptr<IQuantumProcessorQFH> createQuantumProcessorQFHStub() {
    return std::make_unique<QuantumProcessorQFH>();
}

}  // namespace sep::quantum
