#pragma once

#include "core_types/pattern.h"
#include "core_types/candle_data.h"
#include "core_types/result.h"
#include <vector>
#include <string>
#include <memory>

// The Engine Facade - The Brain That the API Layer Observes
// This is the clean measurement boundary between HTTP and core systems

namespace sep::engine {

// Forward declarations to hide implementation details from API
namespace quantum {
    class QuantumProcessor;
    class PatternProcessor;
}

namespace memory {
    class MemoryTierManager;
}

/// Request types for the facade (pure data, no logic)
struct PatternProcessRequest {
    std::vector<core::Pattern> patterns;
    std::string context_id;
    float coherence_threshold{0.5f};
    bool async_processing{false};
};

struct PatternAnalysisRequest {
    std::string pattern_id;
    int analysis_depth{3};
    bool include_relationships{true};
};

struct MemoryQueryRequest {
    std::string query_id;
    int max_results{100};
    float relevance_threshold{0.3f};
};

struct BatchProcessRequest {
    std::vector<core::CandleData> market_data;
    std::string symbol;
    uint64_t timestamp_start{0};
    uint64_t timestamp_end{0};
};

/// Response types for the facade (pure data, no logic)
struct PatternProcessResponse {
    std::vector<core::Pattern> processed_patterns;
    std::string correlation_id;
    float coherence_score{0.0f};
    bool processing_complete{false};
};

struct PatternAnalysisResponse {
    core::Pattern pattern;
    std::vector<core::PatternRelationship> relationships;
    float confidence_score{0.0f};
    std::string analysis_summary;
};

struct MemoryMetricsResponse {
    float stm_utilization{0.0f};
    float mtm_utilization{0.0f};
    float ltm_utilization{0.0f};
    uint64_t total_patterns{0};
    uint64_t active_patterns{0};
    float coherence_level{0.0f};
};

struct HealthStatusResponse {
    bool engine_healthy{false};
    bool quantum_systems_ready{false};
    bool memory_systems_ready{false};
    float cpu_usage{0.0f};
    float memory_usage{0.0f};
    std::string status_message;
};

/// The Engine Facade - Simple Functions That Hide All Complexity
/// The API layer becomes a dumb observer that sends requests and receives responses
class EngineFacade {
public:
    // Lifecycle management
    static EngineFacade& getInstance();
    core::Result initialize();
    core::Result shutdown();
    
    // Core pattern operations
    core::Result processPatterns(const PatternProcessRequest& request, 
                                PatternProcessResponse& response);
    
    core::Result analyzePattern(const PatternAnalysisRequest& request,
                               PatternAnalysisResponse& response);
    
    core::Result processBatch(const BatchProcessRequest& request,
                             PatternProcessResponse& response);
    
    // Memory operations  
    core::Result queryMemory(const MemoryQueryRequest& request,
                            std::vector<core::Pattern>& results);
    
    // System status
    core::Result getHealthStatus(HealthStatusResponse& response);
    core::Result getMemoryMetrics(MemoryMetricsResponse& response);
    
    // Prevent copying/moving
    EngineFacade(const EngineFacade&) = delete;
    EngineFacade& operator=(const EngineFacade&) = delete;
    EngineFacade(EngineFacade&&) = delete;
    EngineFacade& operator=(EngineFacade&&) = delete;

private:
    EngineFacade() = default;
    ~EngineFacade() = default;
    
    // Implementation details hidden from API layer
    struct Impl;
    std::unique_ptr<Impl> impl_;
    bool initialized_{false};
};

} // namespace sep::engine
