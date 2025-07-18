#ifndef SEP_QUANTUM_PATTERN_METRIC_ENGINE_H
#define SEP_QUANTUM_PATTERN_METRIC_ENGINE_H

#include "quantum/processor.h"
#include "quantum/quantum_processor_qfh.h"
#include "core/types.h"
#include <memory>
#include <vector>
#include <istream>
#include <string>
#include <mutex>
#include <unordered_map>

namespace sep::quantum {

/// @brief Holds the computed metrics for a single pattern.
struct PatternMetrics {
    float coherence{0.0f}; ///< Measure of the pattern's internal consistency.
    float stability{0.0f}; ///< Measure of how resistant the pattern is to change.
    float entropy{0.0f};   ///< Measure of the pattern's complexity and randomness.
    std::vector<PatternRelationship> relationships; ///< Relationships to other patterns.
};

/**
 * @class PatternMetricEngine
 * @brief A datatype-agnostic engine for analyzing patterns in raw byte streams.
 *
 * This engine ingests data from any source, treats it as a sequence of bytes,
 * and uses quantum-inspired algorithms to extract and analyze patterns. It is
 * the core component for understanding data in the SEP system without relying
 * on format-specific parsers.
 *
 * @usage
 * @code
 * #include "quantum/pattern_metric_engine.h"
 * #include <vector>
 * #include <iostream>
 *
 * int main() {
 *     PatternMetricEngine engine;
 *     engine.init(nullptr); // Initialize for CPU operation
 *
 *     std::vector<uint8_t> my_data = { ... };
 *     engine.ingestData(my_data.data(), my_data.size());
 *     engine.evolvePatterns();
 *
 *     auto metrics = engine.computeMetrics();
 *     for (const auto& m : metrics) {
 *         std::cout << "Coherence: " << m.coherence << std::endl;
 *     }
 *
 *     return 0;
 * }
 * @endcode
 */
class PatternMetricEngine : public pattern::PatternProcessor {
public:
    explicit PatternMetricEngine();
    ~PatternMetricEngine() override = default;

    /// @brief Initializes the engine and its quantum processing components.
    /// @param ctx A pointer to the GPU context, or `nullptr` for CPU-only operation.
    /// @return SEPResult::SUCCESS on success.
    SEPResult init(quantum::GPUContext* ctx) override;

    /// @brief Ingests a block of data from a raw byte pointer.
    /// @param data Pointer to the data buffer.
    /// @param size Size of the data in bytes.
    void ingestData(const uint8_t* data, size_t size);

    /// @brief Ingests data from a C++ input stream.
    /// @param stream The input stream to read from.
    void ingestData(std::istream& stream);

    /// @brief Ingests a file by reading it in chunks. Suitable for large files.
    /// @param filepath Path to the file to be ingested.
    void ingestFile(const std::string& filepath);

    /// @brief Ingests a file using memory-mapped I/O.
    /// @param filepath Path to the file to be ingested.
    void ingestMappedFile(const std::string& filepath);

    /// @brief Processes the ingested data to identify and evolve patterns.
    void evolvePatterns() override;

    /// @brief Manually adds a pattern to the engine.
    /// @param pattern The pattern to add.
    void addPattern(const pattern::PatternData& pattern);

    /// @brief Creates a mutated version of a given pattern.
    /// @param parent The pattern to mutate.
    /// @return A new PatternData struct representing the mutated pattern.
    pattern::PatternData mutatePattern(const pattern::PatternData& parent) override;

    /// @brief Computes metrics for the currently identified patterns.
    /// @return A vector of PatternMetrics structs.
    std::vector<PatternMetrics> computeMetrics();

private:
    /**
     * @brief Extracts patterns from a raw byte stream.
     *
     * This is the core of the datatype-agnostic design. The algorithm works as follows:
     * 1. The raw byte stream is segmented into fixed-size chunks (e.g., 64 bytes).
     * 2. Each chunk is treated as a potential pattern.
     * 3. A hash is computed for each chunk to quickly identify duplicates.
     * 4. Unique chunks are stored as `PatternData` objects.
     * This approach allows the engine to find repeated sequences in any data type.
     *
     * @param data Pointer to the data buffer.
     * @param size Size of the data in bytes.
     * @return A vector of extracted patterns.
     */
    std::vector<pattern::PatternData> extractPatternsFromBytes(const uint8_t* data, size_t size);
    
    // Quantum processing components
    std::unique_ptr<QuantumProcessorQFH> qfh_processor_;
    
    // Internal pattern storage
    std::vector<pattern::PatternData> current_patterns_;
    std::vector<PatternMetrics> current_metrics_;
};

} // namespace sep::quantum

#endif // SEP_QUANTUM_PATTERN_METRIC_ENGINE_H