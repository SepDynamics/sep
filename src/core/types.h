#ifndef SEP_CORE_TYPES_H
#define SEP_CORE_TYPES_H

#include "memory/types.h"
#include "compat/shim.h"
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include <complex>

namespace sep {

// --- Basic enums and types ---

enum class StreamFlags { Default, NonBlocking };

enum class CompressionMethod { None, DeltaEncoding, LZ4, ZSTD };

namespace pattern {
    enum class PatternResult {
        SUCCESS,
        INVALID_ARGUMENT,
        PROCESSING_ERROR,
        NOT_FOUND
    };

    enum class PatternStateEnum {
        UNINITIALIZED,
        INITIALIZING,
        ACTIVE,
        ERROR,
        STALE
    };
} // namespace pattern

// --- Canonical Data Structures ---

namespace quantum {
    enum class RelationshipType { ENTANGLEMENT, CAUSAL, SIMILARITY };

    struct QuantumState {
        enum class Status { SUPERPOSITION, COHERENT, COLLAPSED };
        float coherence{0.0f};
        float stability{0.0f};
        float entropy{0.0f};
        float phase{0.0f};
        float evolution_rate{0.0f};
        float energy{0.0f};
        float coupling_strength{0.0f};
        float mutation_rate{0.0f};
        int generation{0};
        int mutation_count{0};
        ::sep::memory::MemoryTierEnum memory_tier{::sep::memory::MemoryTierEnum::STM};
        float access_frequency{0.0f};
        Status state{Status::SUPERPOSITION};
    };

    struct PatternRelationship {
      std::string targetId;
      float strength;
      RelationshipType type;
    };
} // namespace quantum

// This is the SINGLE canonical definition of a Pattern.
struct Pattern {
    shim::string id;
    int generation{0};
    glm::vec4 position{0.0f};
    glm::vec4 velocity{0.0f};
    glm::vec4 attributes{0.0f};
    std::complex<float> amplitude{0.0f};
    quantum::QuantumState quantum_state{};
    std::vector<quantum::PatternRelationship> relationships;
    std::vector<float> data;
    std::vector<std::string> parent_ids;
    uint64_t timestamp{0};
    uint64_t last_accessed{0};
    uint64_t last_modified{0};
    glm::vec3 momentum{0.0f};
};

// --- Aliases for Backwards Compatibility ---
// Modules that used different names can use these type aliases.
namespace pattern {
    using PatternData = sep::Pattern;
    using PatternRelationship = sep::quantum::PatternRelationship;
}
namespace workbench {
    using Pattern = sep::Pattern;
}
// Note: The original sep::Pattern is now sep::Pattern

// --- Configuration Structs ---
namespace config {
    struct APIConfig {
        int max_connections{1000};
        int timeout_ms{5000};
        std::string host{"127.0.0.1"};
        uint16_t port{8080};
        uint32_t threads{4};
        size_t keep_alive_timeout_ms{15000};
        std::string log_level{"info"};
        bool enable_metrics{true};
        int max_batch_size{1024};
        struct ResponseModulation {
            bool enabled{true};
            float coherence_threshold{0.7f};
            bool simplify_low_coherence{true};
            int max_detail_level{3};
        } response_modulation{};
        struct OllamaConfig {
            std::string host{"localhost"};
            int port{11434};
            std::string model{"llama2"};
            bool enabled{false};
        } ollama{};
        struct CORSConfig {
            bool enabled{false};
            std::vector<std::string> tokens{};
        } cors{};
        struct RateLimitConfig {
            int requests_per_minute{60};
            bool enabled{true};
        } rate_limit{};
    };

    struct CudaConfig {
        bool use_gpu{true};
        int max_memory_mb{8192};
        int batch_size{1024};
        float gpu_memory_limit{0.9f};
        bool enable_profiling{false};
    };

    struct LogConfig {
        std::string log_level{"info"};
    };

    struct MemoryThresholdConfig {
        float promote_stm_to_mtm{0.7f};
        float promote_mtm_to_ltm{0.9f};
        float demote_threshold{0.3f};
        float fragmentation_threshold{0.3f};
        size_t stm_size{1 << 20};
        size_t mtm_size{4 << 20};
        size_t ltm_size{16 << 20};
        uint32_t stm_to_mtm_min_gen{5};
        uint32_t mtm_to_ltm_min_gen{100};
        bool use_unified_memory{true};
        bool enable_compression{true};
    };

    struct QuantumThresholdConfig {
        float ltm_coherence_threshold{0.9f};
        float mtm_coherence_threshold{0.6f};
        float stability_threshold{0.8f};
    };

    struct SystemConfig {
        APIConfig api{};
        CudaConfig cuda{};
        LogConfig log{};
        MemoryThresholdConfig memory{};
        QuantumThresholdConfig quantum{};
    };

    struct AnalyticsConfig {
        // Fields for analytics go here
    };
} // namespace config

namespace quantum {
    struct ProcessingConfig {
        size_t max_patterns{10000};
        float mutation_rate{0.01f};
        float ltm_coherence_threshold{0.9f};
        float mtm_coherence_threshold{0.6f};
        float stability_threshold{0.8f};
        bool enable_cuda{false};
    };
} // namespace quantum

} // namespace sep

#endif // SEP_CORE_TYPES_H
