#include "quantum/quantum_processor_qfh.h"

namespace sep::quantum {

QuantumProcessorQFH::QuantumProcessorQFH() = default;

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

}  // namespace sep::quantum
