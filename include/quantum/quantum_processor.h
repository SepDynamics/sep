#pragma once

// Compatibility header - redirects to new unified processor
#include "quantum/processor.h"

namespace sep::quantum {

// Forward compatibility - QuantumProcessor is now just Processor
// This allows existing code to continue working
class QuantumProcessor : public Processor {
public:
    using Processor::Processor;
    
    // Legacy Config type mapping
    struct Config {
        size_t max_qubits = 16;
        float decoherence_rate = 0.01f;
        float measurement_threshold = 0.5f;
        bool enable_gpu = false;

        // Convert to new ProcessingConfig
        operator ProcessingConfig() const;
    };
};

} // namespace sep::quantum
