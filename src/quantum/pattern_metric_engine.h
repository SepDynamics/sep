#ifndef SEP_QUANTUM_PATTERN_METRIC_ENGINE_H
#define SEP_QUANTUM_PATTERN_METRIC_ENGINE_H

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <deque>
#include <istream>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

#include "engine/internal/types.h"
#include "quantum/processor.h"
#include "quantum/quantum_processor_qfh.h"
#include "quantum/signal.h"

// Forward declaration to avoid circular includes
namespace sep::quantum
{
    class QuantumProcessorCUDA;
}

namespace sep::quantum
{

    struct SignalThresholds
    {
        float buy_min_coherence{0.7f};
        float buy_min_stability{0.6f};
        float buy_max_entropy{0.3f};
        float sell_max_stability{0.3f};
        float sell_min_entropy{0.7f};
    };

    /// @brief Holds the computed metrics for a single pattern.
    struct PatternMetrics
    {
        char pattern_id[compat::PatternData::MAX_ID_LENGTH];  ///< The ID of the pattern.
        size_t length{0};                                     ///< Number of values in the pattern.
        float coherence{0.0f};  ///< Measure of the pattern's internal consistency.
        float stability{0.0f};  ///< Measure of how resistant the pattern is to change.
        float entropy{0.0f};    ///< Measure of the pattern's complexity and randomness.
        float energy{0.0f};     ///< Sum of squared values representing pattern energy.
        std::vector<PatternRelationship> relationships;  ///< Relationships to other patterns.

        PatternMetrics() { pattern_id[0] = '\0'; }
    };

    /// @brief Aggregated metrics across all current patterns.
    struct AggregateMetrics
    {
        float average_coherence{0.0f};
        float average_stability{0.0f};
        float average_entropy{0.0f};
        float average_energy{0.0f};
    };

    /// @brief Calculate Shannon entropy of a sequence of values.
    /// @param values Input data sequence.
    /// @return Normalized entropy in the range [0,1].
    float calculateEntropy(const std::vector<float>& values);

    /// @brief Calculate pattern stability based on value variance and coherence.
    /// @param values Input data sequence.
    /// @param coherence Previously computed coherence value for the pattern.
    /// @return Normalized stability in the range [0,1].
    float calculateStability(const std::vector<float>& values, float coherence);

    inline float calculateEntropy(const std::vector<float>& values)
    {
        if (values.empty()) return 0.0f;
        float min_v = *std::min_element(values.begin(), values.end());
        float max_v = *std::max_element(values.begin(), values.end());
        if (std::abs(max_v - min_v) < 1e-6f) return 0.0f;
        size_t bins = std::min<size_t>(16, values.size());
        std::vector<size_t> hist(bins, 0);
        float bin_width = (max_v - min_v) / static_cast<float>(bins);
        for (float v : values)
        {
            size_t idx = static_cast<size_t>((v - min_v) / bin_width);
            if (idx >= bins) idx = bins - 1;
            hist[idx]++;
        }
        float entropy = 0.0f;
        for (size_t c : hist)
        {
            if (c == 0) continue;
            float p = static_cast<float>(c) / static_cast<float>(values.size());
            entropy -= p * std::log2(p);
        }
        if (bins > 1) entropy /= std::log2(static_cast<float>(bins));
        return std::clamp(entropy, 0.0f, 1.0f);
    }

    inline float calculateStability(const std::vector<float>& values, float coherence)
    {
        if (values.size() < 2) return 0.0f;

        float mean = 0.0f;
        for (float v : values) mean += v;
        mean /= static_cast<float>(values.size());

        float variance = 0.0f;
        for (float v : values)
        {
            float diff = v - mean;
            variance += diff * diff;
        }
        variance /= static_cast<float>(values.size());

        // Stability is inversely proportional to variance, enhanced by coherence.
        float stability = (1.0f / (1.0f + variance)) * (0.5f + coherence * 0.5f);

        return std::clamp(stability, 0.0f, 1.0f);
    }

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
    class PatternMetricEngine : public pattern::PatternProcessor
    {
    public:
        explicit PatternMetricEngine();
        ~PatternMetricEngine() override = default;

        /// @brief Clears the internal state of the engine.
        void clear();

        /// @brief Initializes the engine and its quantum processing components.
        /// @param ctx A pointer to the GPU context, or `nullptr` for CPU-only operation.
        /// @return SEPResult::SUCCESS on success.
        sep::SEPResult init(quantum::GPUContext* ctx) override;

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
        void addPattern(const compat::PatternData& pattern);

        /// @brief Creates a mutated version of a given pattern.
        /// @param parent The pattern to mutate.
        /// @return A new PatternData struct representing the mutated pattern.
        compat::PatternData mutatePattern(const compat::PatternData& parent) override;

        /// @brief Computes metrics for the currently identified patterns.
        /// @return A vector of PatternMetrics structs.
        const std::vector<PatternMetrics>& computeMetrics() const;

        /// @brief Compute aggregate metrics across all current patterns.
        AggregateMetrics computeAggregateMetrics() const;

        /// @brief Sets the thresholds for signal generation.
        void setSignalThresholds(const SignalThresholds& thresholds);

        /// @brief Set BUY signal thresholds individually
        void setBuyThresholds(float min_coherence, float min_stability, float max_entropy);

        /// @brief Set SELL signal thresholds individually
        void setSellThresholds(float max_stability, float min_entropy);

        /// @brief Returns the current thresholds for signal generation.
        SignalThresholds getSignalThresholds() const { return signal_thresholds_; }

        /// @brief Gets the latest generated signals.
        const std::vector<Signal>& getSignals() const;

        /// @brief Gets the current patterns.
        /// @return A vector of PatternData structs.
        const std::vector<compat::PatternData>& getPatterns() const;

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
        std::vector<compat::PatternData> extractPatternsFromBytes(const uint8_t* data, size_t size);
        void processBuffer(bool is_final_chunk = false);
        void generateSignals();
        SignalType evaluateSignal(const PatternMetrics& metrics) const;

        // Quantum processing components
        std::unique_ptr<QuantumProcessorQFH> qfh_processor_;
        bool use_gpu_;

        // Internal pattern storage with sliding history window
        size_t max_history_size_{1024};
        std::deque<compat::PatternData> current_patterns_;
        mutable std::vector<compat::PatternData> pattern_cache_;
        mutable bool cache_dirty_{false};
        mutable std::vector<PatternMetrics> current_metrics_;
        SignalThresholds signal_thresholds_;
        std::vector<Signal> current_signals_;

        // Preallocated scratch space to avoid frequent allocations
        std::vector<uint32_t> scratch_pattern_bits_;
        mutable std::vector<float> scratch_diffs_;

        // Thread safety and streaming
        mutable std::mutex engine_mutex_;
        std::vector<uint8_t> stream_buffer_;
    };

}  // namespace sep::quantum

#endif  // SEP_QUANTUM_PATTERN_METRIC_ENGINE_H
