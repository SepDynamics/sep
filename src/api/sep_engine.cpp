#include "api/sep_engine.h"

// Standard includes first
#include <chrono>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <sstream>
#include <stdexcept>
#include <atomic>
#include <thread>
#include <vector>

// GLM includes

#include <glm/glm.hpp>

// Project includes
#include "api/types.h"
#include "compat/types.h"
#include "quantum/quantum_processor.h"
#include "memory/memory_tier_manager.hpp"

#include "core/logging.h" // Include logging header first
#include "quantum/types.h" // For quantum::Pattern::generation
#include "compat/math_common.h" // Include math common for sqrt_safe
#include "../../_sep/testbed/context_algorithms.hpp"
#include "embeddings/simple_embedding_model.h"

using json = nlohmann::json;

namespace sep::api {

// Implementation details struct
struct SepEngine::Impl

{
    bool          initialized = false;
    HealthMetrics health_metrics;
    // Using forward declaration instead of direct dependency
    std::unique_ptr<sep::quantum::QuantumProcessor> quantum_processor;
    sep::memory::MemoryTierManager&                 memory_manager;
    std::unique_ptr<sep::pattern::PatternProcessor> pattern_processor; 
    
    // PatternEvolution is a static class, no need to instantiate

    Impl()
        : quantum_processor(sep::quantum::createQuantumProcessor({})) // Use factory function
        , memory_manager(sep::memory::MemoryTierManager::getInstance())
        , pattern_processor(std::make_unique<sep::pattern::PatternProcessor>())
    { 
        // MemoryTierManager uses singleton pattern; store reference for convenience
        health_metrics.startTime           = std::chrono::steady_clock::now();
        health_metrics.lastRequestTime     = std::chrono::steady_clock::now();
        health_metrics.lastSuccessTime     = std::chrono::system_clock::now();
        health_metrics.lastErrorTime       = std::chrono::system_clock::now();
        health_metrics.totalRequests       = 0;
        health_metrics.successfulRequests  = 0;
        health_metrics.failedRequests      = 0;
        health_metrics.timeoutRequests     = 0;
        health_metrics.rateLimitedCount    = 0;
        health_metrics.averageResponseTime = 0.0;
        health_metrics.allocatedMemory     = 0;
        health_metrics.peakMemoryUsage    = 0;
        health_metrics.memoryFragmentation = 0.0;
        health_metrics.lastResponseTime    = std::chrono::milliseconds{0};
        health_metrics.lastErrorCode       = 0;
    }
 
};

// Static member definitions
// id_counter_ uses default sequential consistency
std::atomic<uint64_t> SepEngine::id_counter_{1};

// Singleton instance
SepEngine& SepEngine::getInstance()
{
    static SepEngine instance;
    return instance;
}

// Private constructor
SepEngine::SepEngine() : impl_(std::make_unique<Impl>()) {}

// Private destructor
SepEngine::~SepEngine() = default;

// Generate deterministic ID
std::string SepEngine::generateId(const std::string& prefix)
{
    // fetch_add uses seq_cst semantics
    uint64_t           id = id_counter_.fetch_add(1);
    std::ostringstream oss;
    oss << prefix << "_" << std::setfill('0') << std::setw(8) << id;
    return oss.str();
}

nlohmann::json SepEngine::initialize(const sep::config::APIConfig& /*config*/)
{
    if (impl_->initialized) {
        json result;
        result["success"] = false; 
        result["error"] = "Engine already initialized";
        return result;
    }
        json result;
        result["success"] = true; 
        result["message"] = "SEP Engine initialized successfully";
        return result;

}

nlohmann::json SepEngine::shutdown()
{
        // Clean up components in reverse initialization order
        impl_->quantum_processor.reset();
        impl_->initialized = false;

        json result;
        result["success"] = true;
        result["message"] = "SEP Engine shutdown successfully";
        return result;
}

nlohmann::json SepEngine::processPatterns(const nlohmann::json& request_data)
{
    if (!impl_->initialized) {
        json result;
        result["success"] = false; 
        result["error"]   = "Engine not initialized";
        return result;
    }
    impl_->health_metrics.totalRequests++;
        impl_->health_metrics.lastRequestTime = std::chrono::steady_clock::now();

        // Validate required fields
        json error;
        if (!validateFields(request_data, {"pattern_data"}, error))
        {
            return error;
        }

        // Extract pattern data
        const auto& pattern_data = request_data["pattern_data"];
        if (!pattern_data.is_array() || pattern_data.size() != 3)
        {
            (void)fprintf(stderr, "%s\n", "Invalid pattern data format");
            return makeErrorResponse(api::ErrorCode::InvalidArgument, "Invalid pattern data format");
        }
        glm::vec3 pattern{pattern_data[0].get<float>(), pattern_data[1].get<float>(), pattern_data[2].get<float>()};

        // Generate pattern ID first
        std::string pattern_id = generateId("pat");
        size_t      numeric_id = std::stoull(pattern_id.substr(4));

        // Process through quantum processor
        // Process pattern with proper error handling
        float coherence = 0.0f;
        float stability = 0.0f;
        
        bool process_success = impl_->quantum_processor->processPattern(pattern, numeric_id);
        if (!process_success)
        {
            (void)fprintf(stderr, "%s\n", "Pattern processing failed");
            return makeErrorResponse(api::ErrorCode::SystemError, "Pattern processing failed");
        }

        coherence = impl_->quantum_processor->calculateCoherence(pattern, pattern);
        stability = impl_->quantum_processor->calculateStability(coherence, 0.0f, 0.0f, 1.0f);
        
        // Check for quantum collapse and stability using coherence values
        bool is_collapsed = impl_->quantum_processor->isCollapsed(coherence);
        bool is_stable    = impl_->quantum_processor->isStable(coherence);

        impl_->health_metrics.successfulRequests++;
        impl_->health_metrics.lastSuccessTime = std::chrono::system_clock::now();

        json result;
        result["success"]      = true;
        result["pattern_id"]   = pattern_id;
        result["coherence"]    = coherence;
        result["stability"]    = stability;
        result["is_collapsed"] = is_collapsed;
        result["is_stable"]    = is_stable;
        return result;

}

nlohmann::json SepEngine::processBatch(const nlohmann::json& request_data)
{
    if (!impl_->initialized) {
        json result;
        result["success"] = false; 
        result["error"]   = "Engine not initialized";
        return result;
    }
    impl_->health_metrics.totalRequests++;
        impl_->health_metrics.lastRequestTime = std::chrono::steady_clock::now();

        if (!request_data.contains("patterns") || !request_data["patterns"].is_array())
        {
            json result;
            result["success"] = false;
            result["error"]   = "Missing patterns array";
            return result;
        }

        std::string batch_id = generateId("batch");
        json        results  = json::array();

        for (const auto& p : request_data["patterns"])
        {
            if (!p.is_array() || p.size() != 3)
                continue;
            glm::vec3 pattern{p[0].get<float>(), p[1].get<float>(), p[2].get<float>()};

            // Generate ID first
            std::string id         = generateId("pat");
            size_t      numeric_id = std::stoull(id.substr(4));

            // Process pattern with numeric ID
            bool process_success = impl_->quantum_processor->processPattern(pattern, numeric_id);

            // Only proceed if processing succeeded
            if (process_success)
            {
                float coherence = impl_->quantum_processor->calculateCoherence(pattern, pattern);
                
                float stability = impl_->quantum_processor->calculateStability(coherence, 0.0f, 1.0f, 1.0f); // Use dummy values for history, generation, access_frequency
 
                // Check states using coherence values
                bool collapsed = impl_->quantum_processor->isCollapsed(coherence);
                bool stable    = impl_->quantum_processor->isStable(coherence);

                json entry;
                entry["pattern_id"]   = id;
                entry["coherence"]    = coherence;
                entry["stability"]    = stability;
                entry["is_collapsed"] = collapsed;
                entry["is_stable"]    = stable;
                results.push_back(entry);
            }
        }

        impl_->health_metrics.successfulRequests++;
        impl_->health_metrics.lastSuccessTime = std::chrono::system_clock::now();

        json result;
        result["success"]  = true;
        result["batch_id"] = batch_id;
        result["results"]  = results;
        return result;
}

nlohmann::json SepEngine::validateContexts(const nlohmann::json& request_data)
{
    if (!impl_->initialized) {
        json result;
        result["success"] = false;
        result["error"]   = "Engine not initialized";
        return result;
    }

    impl_->health_metrics.totalRequests++;

    if (!request_data.contains("contexts") || !request_data["contexts"].is_array())
    {
        return makeErrorResponse(api::ErrorCode::InvalidArgument, "Missing contexts array");
    }

    auto report = sep::testbed::validate_contexts(request_data["contexts"]);

    impl_->health_metrics.successfulRequests++;

    json result;
    result["success"]         = true;
    result["valid"]           = report.overall_valid;
    result["context_count"]   = request_data["contexts"].size();
    result["invalid_indices"] = report.invalid_indices;
    return result;
}

nlohmann::json SepEngine::getPatternHistory(const nlohmann::json& request_data)
{
    if (!impl_->initialized) {
        json result;
        result["success"] = false; 
        result["error"]   = "Engine not initialized";
        return result;
    }

    // Extract optional filter parameters
    float min_coherence = request_data.value("min_coherence", 0.0f);
    float min_stability = request_data.value("min_stability", 0.0f);
    
    json history = json::array();
    const auto& patterns = impl_->pattern_processor->getPatterns();
    for (const auto& p : patterns) { 
        // Apply filters if specified
        
        if (p.coherence >= min_coherence && p.stability >= min_stability) {
            json e;
            e["coherence"] = p.coherence;
            e["stability"] = p.stability;
            history.push_back(e);
        }
    }

        json result; 
        result["success"] = true;
        result["history"] = history;
        return result;
}

nlohmann::json SepEngine::extractEmbeddings(const nlohmann::json& request_data)
{
    if (!impl_->initialized) {
        json result;
        result["success"] = false;
        result["error"]   = "Engine not initialized";
        return result;
    }
    impl_->health_metrics.totalRequests++;

    if (!request_data.contains("text") || !request_data["text"].is_string()) {
        return makeErrorResponse(api::ErrorCode::InvalidArgument,
                                 "Missing text field");
    }

    static sep::embeddings::SimpleEmbeddingModel model;
    std::vector<double> embeddings = model.compute(request_data["text"].get<std::string>());

    impl_->health_metrics.successfulRequests++;

    json result;
    result["success"]    = true;
    result["embeddings"] = embeddings;
    return result;
}

nlohmann::json SepEngine::calculateSimilarity(const nlohmann::json& request_data)
{
    if (!impl_->initialized) {
        json result;
        result["success"] = false; 
        result["error"]   = "Engine not initialized";
        return result;
    }

        if (!request_data.contains("embedding1") || !request_data.contains("embedding2"))
        {
            json result;
            result["success"] = false; 
            result["error"]   = "Missing required embeddings";
            return result;
        }

        const auto& emb1 = request_data["embedding1"];
        const auto& emb2 = request_data["embedding2"];

        if (emb1.size() != emb2.size())
        {
            json result;
            result["success"] = false; 
            result["error"]   = "Embeddings must have the same dimension";
            return result;
        }

        // Calculate cosine similarity
        double dot_product = 0.0;
        double norm1       = 0.0;
        double norm2       = 0.0;

        for (size_t i = 0; i < emb1.size(); ++i)
        {
            double val1 = emb1[i].get<double>();
            double val2 = emb2[i].get<double>();
            dot_product += val1 * val2;
            norm1 += val1 * val1;
            norm2 += val2 * val2;
        }

        // Use sep::math::sqrt_safe instead of std::sqrt to avoid CUDA/glibc conflicts
        double similarity = dot_product / (sep::math::sqrt_safe(norm1) * sep::math::sqrt_safe(norm2));

        json result;
        result["success"]    = true; 
        result["similarity"] = similarity;
        return result;
}

nlohmann::json SepEngine::blendContexts(const nlohmann::json& request_data)
{
    if (!impl_->initialized) {
        json result;
        result["success"] = false;
        result["error"]   = "Engine not initialized";
        return result;
    }

    if (!request_data.contains("contexts") || !request_data["contexts"].is_array())
    {
        json result;
        result["success"] = false;
        result["error"]   = "contexts must be an array";
        return result;
    }

    std::vector<std::vector<double>> embeddings;
    std::vector<double>           timestamps;
    for (size_t idx = 0; idx < request_data["contexts"].size(); ++idx)
    {
        const auto& ctx = request_data["contexts"][idx];
        if (!ctx.contains("content") || !ctx["content"].is_array() ||
            !ctx.contains("metadata") || !ctx["metadata"].is_object() ||
            !ctx["metadata"].contains("timestamp"))
        {
            json result;
            result["success"] = false;
            result["error"]   = std::string("invalid context at index ") + std::to_string(idx);
            return result;
        }
        std::vector<double> emb;
        emb.reserve(ctx["content"].size());
        for (const auto& v : ctx["content"])
            emb.push_back(v.get<double>());
        embeddings.push_back(std::move(emb));
        timestamps.push_back(ctx["metadata"]["timestamp"].get<double>());
    }

    size_t dim = embeddings[0].size();
    for (const auto& e : embeddings)
    {
        if (e.size() != dim)
        {
            json result;
            result["success"] = false;
            result["error"]   = "inconsistent embedding dimensions";
            return result;
        }
    }

    std::vector<double> weights;
    if (request_data.contains("weights"))
    {
        if (!request_data["weights"].is_array() ||
            request_data["weights"].size() != embeddings.size())
        {
            json result;
            result["success"] = false;
            result["error"]   = "weights size mismatch";
            return result;
        }
        for (const auto& w : request_data["weights"])
            weights.push_back(w.get<double>());
    }

    // Normalize weights and compute timestamp blend
    if (weights.empty())
        weights.assign(embeddings.size(), 1.0 / static_cast<double>(embeddings.size()));

    double sum_w = 0.0;
    for (double v : weights) sum_w += v;
    if (sum_w == 0.0)
    {
        json result;
        result["success"] = false;
        result["error"]   = "weights sum to zero";
        return result;
    }
    for (double& v : weights) v /= sum_w;

    auto blend_report = sep::testbed::blend_embeddings(embeddings, weights);
    if (!blend_report.success)
    {
        json result;
        result["success"] = false;
        result["error"]   = blend_report.error;
        return result;
    }

    double ts = 0.0;
    for (size_t i = 0; i < timestamps.size(); ++i)
        ts += timestamps[i] * weights[i];

    json blend;
    blend["embedding"] = blend_report.blended;
    blend["coherence"] = blend_report.coherence;
    blend["metadata"]  = { {"timestamp", ts} };
    blend["type"]       = "blended";
    blend["blended_context_id"] = generateId("blend");

    json result;
    result["success"] = true;
    result["result"]  = blend;
    return result;
}

nlohmann::json SepEngine::getHealthStatus()
{
    if (!impl_->initialized) {
        json result;
        result["success"] = false; 
        result["error"]   = "Engine not initialized";
        return result;
    }
    auto now    = std::chrono::steady_clock::now();
    auto uptime = std::chrono::duration_cast<std::chrono::seconds>(
        now - impl_->health_metrics.startTime)
                       .count();

    auto metrics_json = getMetrics(impl_->health_metrics);

    json result;
    result["success"]        = true; 
    result["status"]         = "healthy";
    result["uptime_seconds"] = uptime;
    result["initialized"]    = impl_->initialized;
    result["metrics"]        = metrics_json;
    return result;
}

nlohmann::json SepEngine::getMemoryMetrics()
{
    auto& engine = SepEngine::getInstance();
    if (!engine.impl_->initialized) {
        json result; 
        result["success"] = false;
        result["error"]   = "Engine not initialized";
        return result;
    }

        // Mock memory statistics
        json stm_tier;
        stm_tier["total_size"]     = 1024;
        stm_tier["allocated_size"] = 512;
        stm_tier["utilization"]    = 0.5;

        json mtm_tier;
        mtm_tier["total_size"]     = 2048;
        mtm_tier["allocated_size"] = 1024;
        mtm_tier["utilization"]    = 0.5;

        json ltm_tier;
        ltm_tier["total_size"]     = 4096;
        ltm_tier["allocated_size"] = 2048;
        ltm_tier["utilization"]    = 0.5;

        json memory_tiers;
        memory_tiers["STM"] = stm_tier;
        memory_tiers["MTM"] = mtm_tier;
        memory_tiers["LTM"] = ltm_tier;

        json result;
        result["success"]      = true; 
        result["memory_tiers"] = memory_tiers;
        return result;
}

nlohmann::json SepEngine::getConfig(const sep::config::APIConfig& config)
{
    auto& engine = SepEngine::getInstance();
    if (!engine.impl_->initialized) {
        json result; 
        result["success"] = false;
        result["error"]   = "Engine not initialized";
        return result;
    }

        json cors_config;
        cors_config["enabled"] = config.cors.enabled;

        json rate_limit_config;
        rate_limit_config["enabled"]             = config.rate_limit.enabled;
        rate_limit_config["requests_per_minute"] = config.rate_limit.rpm;

        json api_config;
        api_config["port"]       = config.port;
        api_config["threads"]    = config.threads;
        api_config["log_level"]  = config.log_level;
        api_config["cors"]       = cors_config;
        api_config["rate_limit"] = rate_limit_config;

        json quantum_config;
        quantum_config["processor_type"] = "mock";
        quantum_config["max_qubits"]     = 32;

        json memory_config;
        memory_config["stm_ttl_hours"]   = 1;
        memory_config["mtm_ttl_days"]    = 7;
        memory_config["ltm_compression"] = true;

        json config_json;
        config_json["api"]     = api_config; 
        config_json["quantum"] = quantum_config;
        config_json["memory"]  = memory_config;

        json result;
        result["success"] = true;
        result["config"]  = config_json;
        return result;
}

nlohmann::json SepEngine::makeErrorResponse(api::ErrorCode code, const std::string& message)
{
    nlohmann::json result;
    result["success"] = false;
    result["error"]["code"] = static_cast<int>(code);
    result["error"]["message"] = message;
    return result;
}

bool SepEngine::validateFields(const nlohmann::json&           data,
                               const std::vector<std::string>& fields,
                               nlohmann::json&                 error)
{
    for (const auto& field : fields) {
        if (!data.contains(field)) {
            error = makeErrorResponse(api::ErrorCode::InvalidArgument, "Missing field: " + field);
            return false;
        }
    }
    return true;
}

nlohmann::json SepEngine::getMetrics(const HealthMetrics& metrics)
{
    using json = nlohmann::json;
    using namespace std::chrono;
    
    auto now = steady_clock::now();
    auto uptime = duration_cast<seconds>(now - metrics.startTime).count();

    json result = {
        {"uptime_seconds", uptime},
        {"requests", {
            {"total", metrics.totalRequests.load()},
            {"successful", metrics.successfulRequests.load()},
            {"failed", metrics.failedRequests.load()},
            {"timeout", metrics.timeoutRequests.load()},
            {"rate_limited", metrics.rateLimitedCount.load()}
        }},
        {"response_time", {
            {"average", metrics.averageResponseTime.load()},
            {"last", metrics.lastResponseTime.count()}
        }},
        {"timestamps", {
            {"last_request", duration_cast<seconds>(metrics.lastRequestTime.time_since_epoch()).count()},
            {"last_success", duration_cast<seconds>(metrics.lastSuccessTime.time_since_epoch()).count()}
        }},
        {"memory", {
            {"allocated_bytes", metrics.allocatedMemory.load()},
            {"peak_bytes", metrics.peakMemoryUsage.load()},
            {"fragmentation", metrics.memoryFragmentation.load()}
        }}
    };
    
    return result;
}

}  // namespace sep::api
