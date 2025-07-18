#ifndef SEP_QUANTUM_PATTERN_METRIC_ENGINE_H
#define SEP_QUANTUM_PATTERN_METRIC_ENGINE_H

#include "quantum/processor.h"
#include "quantum/quantum_processor_qfh.h"
#include "core/types.h"
#include <memory>
#include <vector>
#include <istream>

namespace sep::quantum {

struct PatternMetrics {
    float coherence{0.0f};
    float stability{0.0f};
    float entropy{0.0f};
    std::vector<PatternRelationship> relationships;
};

class PatternMetricEngine : public pattern::PatternProcessor {
public:
    explicit PatternMetricEngine();
    ~PatternMetricEngine() override = default;

    // Initialize the engine with GPU context
    SEPResult init(quantum::GPUContext* ctx) override;

    // Datatype-agnostic ingestion methods
    void ingestData(const uint8_t* data, size_t size);
    void ingestData(std::istream& stream);

    // Pattern evolution and metrics computation
    void evolvePatterns() override;
    pattern::PatternData mutatePattern(const pattern::PatternData& parent) override;
    std::vector<PatternMetrics> computeMetrics();

private:
    // Convert raw bytes to pattern data
    std::vector<pattern::PatternData> extractPatternsFromBytes(const uint8_t* data, size_t size);
    
    // Quantum processing components
    std::unique_ptr<QuantumProcessorQFH> qfh_processor_;
    
    // Internal pattern storage
    std::vector<pattern::PatternData> current_patterns_;
    std::vector<PatternMetrics> current_metrics_;
};

} // namespace sep::quantum

#endif // SEP_QUANTUM_PATTERN_METRIC_ENGINE_H