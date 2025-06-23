#include "compat/stream.h"
#include "compat/memory.h"
#include "quantum/priority.h"
#include "quantum/relationship.h"
#include "quantum/pattern_processor.h"
#include "core/types.h"

#include "memory/memory_tier_manager.hpp"
#include "compat/math_common.h"


#include "compat/shim.h"

#include <algorithm>  // For std::clamp, std::min, std::max
#include <chrono>
#include <cstring>
#include <deque>
#include <fstream>
#include <glm/glm.hpp>
#include <memory>
#include <mutex>
#include <numeric>
#include <spdlog/spdlog.h>
#include <string>
#include <utility>
#include <vector>
#include <nlohmann/json.hpp>


#ifndef SEP_HAS_EXCEPTIONS
#    if defined(__cpp_exceptions) || defined(__EXCEPTIONS) || defined(_CPPUNWIND)
#        define SEP_HAS_EXCEPTIONS 1
#    else
#        define SEP_HAS_EXCEPTIONS 0
#    endif
#endif

#ifdef __linux__
#    include <sys/sysinfo.h>
#endif


#include "quantum/processor.h"
#include "quantum/resource_predictor.h"
#include "quantum/pattern_evolution.h"
#include "quantum/types.h"
#include "memory/manager.h"
#include "core/allocation_metrics.h"

namespace sep::context {

// Helper function for memory tier determination
sep::MemoryTierEnum determineTierFromScore(float score)
{
    // STM: <0.7 coherence
    // MTM: 0.7-0.89 coherence
    // LTM: ≥0.9 coherence
    if (score >= 0.9f)
    {
        return sep::MemoryTierEnum::LTM;
    }
    else if (score >= 0.7f)
    {
        return sep::MemoryTierEnum::MTM;
    }
    return sep::MemoryTierEnum::STM;
}

// Helper functions for result types
EmbeddingResult makeEmbeddingError(const std::string& msg)
{
    EmbeddingResult result;
    result.success = false;
    result.error = sep::shim::string(msg.c_str());
    return result;
}

SimilarityResult makeSimilarityError(const std::string& msg)
{
    SimilarityResult result;
    result.success = false;
    result.error = sep::shim::string(msg.c_str());
    return result;
}

BlendResult makeBlendError(const std::string& msg)
{
    BlendResult result;
    result.success = false;
    result.error = sep::shim::string(msg.c_str());
    return result;
}

std::vector<float> adjustEmbeddingSize(std::vector<float> emb, std::size_t target)
{
    if (target == 0)
        return emb;
    if (emb.size() > target)
    {
        emb.resize(target);
    }
    else if (emb.size() < target)
    {
        emb.insert(emb.end(), target - emb.size(), 0.0f);
    }
    return emb;
}

std::vector<float> simpleTextEmbedding(const std::string& text, std::size_t dim)
{
    if (dim == 0)
        dim = 32;
    std::vector<float> emb(dim, 0.0f);
    for (size_t i = 0; i < text.size(); ++i)
    {
        unsigned char c = static_cast<unsigned char>(text[i]);
        emb[c % dim] += 1.0f;
    }
    float norm = 0.0f;
    for (float v : emb)
        norm += v * v;
    norm = sep::math::sqrt_safe(norm);
    if (norm > 0.0f)
    {
        for (auto& v : emb)
            v /= norm;
    }
    return emb;
}

void flattenJson(const nlohmann::json& j, std::string& out)
{
    if (j.is_string())
    {
        out += j.get<std::string>();
        out += ' ';
    }
    else if (j.is_number())
    {
        out += std::to_string(j.get<double>());
        out += ' ';
    }
    else if (j.is_boolean())
    {
        out += j.get<bool>() ? "true " : "false ";
    }
    else if (j.is_array())
    {
        for (const auto& el : j)
        {
            flattenJson(el, out);
        }
    }
    else if (j.is_object())
    {
        for (auto it = j.begin(); it != j.end(); ++it)
        {
            out += it.key();
            out += ' ';
            flattenJson(it.value(), out);
        }
    }
}

std::vector<float> simpleJsonEmbedding(const nlohmann::json& j, std::size_t dim)
{
    std::string text;
    flattenJson(j, text);
    return simpleTextEmbedding(text, dim);
}

class ProcessorImpl : public Processor
{
public:
    explicit ProcessorImpl(const ProcessOptions& options)
        : options_(options)
        , logger_(sep::logging::Manager::getInstance().createLogger("context_processor", sep::logging::LoggerConfig{}))
        , stream_(std::make_shared<cuda::Stream>())
        , device_buffer_(std::make_shared<cuda::DeviceMemory<float>>())
        , priority_manager_(PriorityConfig{})
        , relationship_manager_(RelationshipConfig{})
        , pattern_processor_(std::make_unique<pattern::PatternProcessor>())
    {
        logger_->info("Context processor initialized");
    }

    ProcessorImpl(const ProcessorImpl&)                = delete;
    ProcessorImpl& operator=(const ProcessorImpl&)     = delete;
    ProcessorImpl(ProcessorImpl&&) noexcept            = delete;
    ProcessorImpl& operator=(ProcessorImpl&&) noexcept = delete;

    ~ProcessorImpl()
    {
#if defined(__cpp_exceptions)
        try
        {
            if (logger_)
            {
                logger_->info("Context processor destroyed");
            }
        }
        catch (...)
        {
            // Suppress logging errors during destruction
        }
#else
        if (logger_)
        {
            logger_->info("Context processor destroyed");
        }
#endif
    }

    ProcessResult processContext(const Context& context) override
    {
#if defined(__cpp_exceptions)
        try
        {
#endif
            ValidationResult validation = validateContext(context);
            if (!validation.valid)
            {
                return ProcessResult::fail(validation.error);
            }

            auto embeddings = extractEmbeddings(context);
            if (!embeddings.success)
            {
                return ProcessResult::fail(embeddings.error);
            }

            float score = calculateStabilityScore(embeddings.value);

            auto&                     mem_mgr = sep::memory::MemoryTierManager::getInstance();
            sep::MemoryTierEnum       tier    = determineTierFromScore(score);
            sep::memory::MemoryBlock* block   = mem_mgr.allocate(embeddings.value.size() * sizeof(float), static_cast<sep::memory::TierType>(tier));
            if (!block)
            {
                {
                    std::lock_guard<std::mutex> lock(metrics_mutex_);
                    metrics_.allocation_failures++;
                }
                metrics::allocationFailures().value++;
                return ProcessResult::fail("Memory allocation failed");
            }

            std::memcpy(block->ptr, embeddings.value.data(), embeddings.value.size() * sizeof(float));
            mem_mgr.updateBlockMetrics(block, score, score, 1, score);
            mem_mgr.optimizeBlocks();
            mem_mgr.optimizeTiers();

            // Update pattern processor with embedding information
            pattern::PatternData pd;
            pd.id         = sep::shim::string(getContextId(context).c_str());
            pd.coherence  = score;
            pd.stability  = score;
            pd.attributes = glm::vec4(0.0f);
            if (!embeddings.value.empty())
            {
                pd.attributes.x = embeddings.value[0];
                if (embeddings.value.size() > 1)
                    pd.attributes.y = embeddings.value[1];
                if (embeddings.value.size() > 2)
                    pd.attributes.z = embeddings.value[2];
                if (embeddings.value.size() > 3)
                    pd.attributes.w = embeddings.value[3];
            }
            pattern_processor_->addPattern(pd);
            const auto& updated_patterns = pattern_processor_->process();

            nlohmann::json pattern_array = nlohmann::json::array();
            for (const auto& p : updated_patterns)
            {
                pattern_array.push_back(quantum::mcp::PatternEvolution::toJson(p));
            }

            // Cast away constness to attach results to the context
            Context& mutable_ctx                    = const_cast<Context&>(context);
            mutable_ctx.processorResult["patterns"] = std::move(pattern_array);

            std::vector<CheckResult> results_std = {CheckResult{CheckResult::Status::STABLE, score, ""}};
            ::sep::shim::vector<CheckResult> results;
            results.reserve(results_std.size());
            for (const auto& r : results_std)
                results.push_back(r);
            return ProcessResult::ok(std::move(results));
#if defined(__cpp_exceptions)
        }
        catch (const std::exception& e)
        {
            logger_->error("Error processing context: {}", e.what());
            return ProcessResult::fail(e.what());
        }
#endif
    }

    ProcessResult processBatch(const Batch& batch) override
    {
#if defined(__cpp_exceptions)
        try
        {
#endif
            ::sep::shim::vector<CheckResult> results;
            results.reserve(batch.contexts.size());
            auto start = std::chrono::high_resolution_clock::now();

            for (const auto& context : batch.contexts)
            {
                auto result = processContext(context);
                if (!result.success)
                {
                    return result;
                }
                results.insert(results.end(), result.value.begin(), result.value.end());

                // Record batch processing for resource prediction
                if (result.success)
                {
                    std::string context_id = getContextId(context);
                    recordContextAccess(context_id, context);
                }
            }

            auto end      = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration<double>(end - start).count();
            updateMetrics(results.size(), duration);

            return ProcessResult::ok(std::move(results));
#if defined(__cpp_exceptions)
        }
        catch (const std::exception& e)
        {
            logger_->error("Error processing batch: {}", e.what());
            return ProcessResult::fail(e.what());
        }
#endif
    }

    ValidationResult validateContext(const Context& context) override
    {
        std::string error;
        if (!validateSchema(context, error))
        {
            return ValidationResult{false, sep::shim::string(error.c_str()), {}};
        }
        if (!validateMetadata(context, error))
        {
            return ValidationResult{false, sep::shim::string(error.c_str()), {}};
        }

        // Multi-layer validation with custom scoring
        float relevance_score = calculateRelevanceScore(context);
        if (relevance_score < 0.4f)
        {  // Minimum relevance threshold
            return ValidationResult{false, sep::shim::string("Context relevance below threshold"), {}};
        }

        return ValidationResult{true, sep::shim::string(""), {}};
    }

    EmbeddingResult extractEmbeddings(const Context& context) override
    {
#if defined(__cpp_exceptions)
        try
        {
#endif
            const Context*     ctx = &context;
            std::vector<float> embeddings;

            // Direct embedding provided as context content
            if (ctx->type == "embedding" && ctx->content.is_array())
            {
                embeddings = ctx->content.get<std::vector<float>>();
            }
            else if (ctx->content.is_object())
            {
                // Common fields used to store embeddings
                if (ctx->content.contains("embedding") && ctx->content["embedding"].is_array())
                {
                    embeddings = ctx->content["embedding"].get<std::vector<float>>();
                }
                else if (ctx->content.contains("embeddings") && ctx->content["embeddings"].is_array()
                         && !ctx->content["embeddings"].empty() && ctx->content["embeddings"][0].is_array())
                {
                    embeddings = ctx->content["embeddings"][0].get<std::vector<float>>();
                }
            }

            if (embeddings.empty() && ctx->processorResult.contains("embeddings")
                && ctx->processorResult["embeddings"].is_array() && !ctx->processorResult["embeddings"].empty())
            {
                embeddings = ctx->processorResult["embeddings"][0].get<std::vector<float>>();
            }

            if (embeddings.empty())
            {
                std::string text;
                if (ctx->content.is_string())
                {
                    text = ctx->content.get<std::string>();
                }
                else if (ctx->content.is_object() && ctx->content.contains("text") && ctx->content["text"].is_string())
                {
                    text = ctx->content["text"].get<std::string>();
                }
                else if (ctx->content.is_object() || ctx->content.is_array())
                {
                    embeddings = simpleJsonEmbedding(ctx->content, options_.embedding_size);
                }

                if (embeddings.empty() && !text.empty())
                {
                    embeddings = simpleTextEmbedding(text, options_.embedding_size);
                }
            }

            if (embeddings.empty())
            {
                return makeEmbeddingError("No embeddings found");
            }

            embeddings = adjustEmbeddingSize(embeddings, options_.embedding_size);

            ::sep::shim::vector<float> shim_emb;
            shim_emb.reserve(embeddings.size());
            for (float v : embeddings)
                shim_emb.push_back(v);

            return EmbeddingResult{true, shim_emb, sep::shim::string("")};
#if defined(__cpp_exceptions)
        }
        catch (const std::exception& e)
        {
            return makeEmbeddingError(e.what());
        }
#endif
    }

    SimilarityResult calculateSimilarity(const Context& a, const Context& b) override
    {
#if defined(__cpp_exceptions)
        try
        {
#endif
            auto emb_a = extractEmbeddings(a);
            auto emb_b = extractEmbeddings(b);

            if (!emb_a.success || !emb_b.success)
            {
                return makeSimilarityError("Failed to extract embeddings");
            }

            if (emb_a.value.size() != emb_b.value.size() || emb_a.value.empty())
            {
                return makeSimilarityError("Incompatible embeddings");
            }

            std::string id_a = getContextId(a);
            std::string id_b = getContextId(b);

            std::vector<float> va(emb_a.value.begin(), emb_a.value.end());
            std::vector<float> vb(emb_b.value.begin(), emb_b.value.end());
            float similarity = relationship_manager_.storeCosineSimilarity(id_a, id_b, va, vb);
            if (similarity == 0.0f)
            {
                similarity = relationship_manager_.calculateRelationshipStrength(va, vb);
            }

            std::string pattern;
            std::string text;
            if (a.content.is_string())
                pattern = a.content.get<std::string>();
            if (b.content.is_string())
                text = b.content.get<std::string>();
            if (!pattern.empty() && !text.empty())
            {
                relationship_manager_.storePatternMatch(id_a, id_b, pattern, text);
            }

            // Create or update relationship if similarity is above threshold
            if (similarity >= 0.3f)
            {
                relationship_manager_.addRelationship(id_a, id_b, static_cast<uint8_t>(0), similarity, true); // 0 = REFERENCE
            }

            return SimilarityResult{true, similarity, sep::shim::string("")};
#if defined(__cpp_exceptions)
        }
        catch (const std::exception& e)
        {
            return makeSimilarityError(e.what());
        }
#endif
    }

    BlendResult blendContexts(const ::sep::shim::vector<Context>& contexts,
                              const ::sep::shim::vector<float>& weights) override

    {
#if defined(__cpp_exceptions)
        try
        {
#endif
            std::vector<Context> ctxs(contexts.begin(), contexts.end());
            std::vector<float>  wts(weights.begin(), weights.end());

            if (ctxs.empty() || ctxs.size() != wts.size())
            {
                return makeBlendError("Invalid input");
            }

            std::vector<std::vector<float>> all_embeddings;
            all_embeddings.reserve(ctxs.size());
            for (const auto& ctx : ctxs)
            {
                auto emb = extractEmbeddings(ctx);
                if (!emb.success)
                {
                    return makeBlendError("Failed to extract embeddings");
                }
                std::vector<float> tmp(emb.value.begin(), emb.value.end());
                all_embeddings.push_back(std::move(tmp));
            }

            if (all_embeddings.empty() || all_embeddings[0].empty())
            {
                return makeBlendError("No embeddings to blend");
            }

            size_t             dim = all_embeddings[0].size();
            std::vector<float> blended(dim, 0.0f);
            float              weight_sum = 0.0f;

            for (size_t i = 0; i < all_embeddings.size(); ++i)
            {
                if (all_embeddings[i].size() != dim)
                {
                    return makeBlendError("Inconsistent embedding dimensions");
                }
                float w = wts[i];
                weight_sum += w;
                for (size_t j = 0; j < dim; ++j)
                {
                    blended[j] += all_embeddings[i][j] * w;
                }
            }

            if (weight_sum != 0.0f)
            {
                for (auto& v : blended)
                    v /= weight_sum;
            }

            Context result = ctxs[0];
            result.type    = "embedding";
            result.content = blended;

            return BlendResult{true, std::move(result), sep::shim::string("")};
#if defined(__cpp_exceptions)
        }
        catch (const std::exception& e)
        {
            return makeBlendError(e.what());
        }
#endif
    }

private:
    bool validateSchema(const Context& context, std::string& error)
    {
        if (!context.content.is_array() && !context.content.is_object())
        {
            error = "Context content must be an array or object";
            return false;
        }
        return true;
    }

    bool validateMetadata(const Context& context, std::string& error)
    {
        if (!context.metadata.contains("timestamp"))
        {
            error = "Missing required metadata field: timestamp";
            return false;
        }
        return true;
    }

    float calculateStabilityScore(const ::sep::shim::vector<float>& embeddings)
    {
#if defined(__cpp_exceptions)
        try
        {
#endif
            if (embeddings.empty())
            {
                return 0.0f;
            }
            double mean = 0.0;
            for (float v : embeddings)
            {
                mean += static_cast<double>(v);
            }
            mean /= static_cast<double>(embeddings.size());

            double var = 0.0;
            for (float v : embeddings)
            {
                double diff = static_cast<double>(v) - mean;
                var += diff * diff;
            }
            var /= static_cast<double>(embeddings.size());

            double stddev = sep::math::sqrt_safe(var);
            float  cv_f   = sep::math::to_float(stddev / (std::abs(mean) + 1e-6));
            float  score  = 1.0f / (1.0f + cv_f);
            score        = std::clamp(score, 0.0f, 1.0f);
            return score;
#if defined(__cpp_exceptions)
        }
        catch (...)
        {
            return 0.0f;
        }
#endif
    }

    void updateMetrics(size_t processed_count, double processing_time)
    {
        std::lock_guard<std::mutex> lock(metrics_mutex_);
        metrics_.contexts_processed += processed_count;
        metrics_.batches_processed++;
        metrics_.average_processing_time =
            (metrics_.average_processing_time * (metrics_.batches_processed - 1) + processing_time)
            / metrics_.batches_processed;
    }

    ProcessorMetrics getMetrics() const override
    {
        std::lock_guard<std::mutex> lock(metrics_mutex_);
        return metrics_;
    }

    void setHooks(sep::core::SystemHooks* hooks) override
    {
        hooks_ = hooks;
    }

    ProcessOptions                             options_;
    std::shared_ptr<spdlog::logger>            logger_;
    std::shared_ptr<cuda::Stream>              stream_;
    std::shared_ptr<cuda::DeviceMemory<float>> device_buffer_;
    PriorityManager                            priority_manager_;
    RelationshipManager                        relationship_manager_;
    std::unique_ptr<pattern::PatternProcessor> pattern_processor_;
    mutable std::mutex                         metrics_mutex_;
    ProcessorMetrics                           metrics_{};
    sep::core::SystemHooks*                    hooks_{nullptr};

private:
    // Helper functions moved inside the class
    std::string getContextId(const Context& context)
    {
        // Generate a simple hash-based ID from context content
        std::hash<std::string> hasher;
        std::string            content_str = context.content.dump();
        return std::to_string(hasher(content_str));
    }

    float calculateRelevanceScore(const Context& context)
    {
#if defined(__cpp_exceptions)
        try
        {
#endif
            // Calculate relevance based on content complexity and metadata
            float base_score = 0.5f;

            // Boost score based on content size and structure
            if (context.content.is_object())
            {
                base_score += 0.2f;
                base_score += std::min(0.3f, static_cast<float>(context.content.size()) * 0.05f);
            }
            else if (context.content.is_array())
            {
                base_score += 0.1f;
                base_score += std::min(0.2f, static_cast<float>(context.content.size()) * 0.02f);
            }

            // Boost score based on metadata richness
            if (!context.metadata.empty())
            {
                base_score += std::min(0.2f, static_cast<float>(context.metadata.size()) * 0.03f);
            }

            return std::clamp(base_score, 0.0f, 1.0f);
#if defined(__cpp_exceptions)
        }
        catch (...)
        {
            return 0.4f;  // Default relevance score
        }
#endif
    }

    void recordContextAccess(const std::string& context_id, const Context& /*context*/)
    {
#if defined(__cpp_exceptions)
        try
        {
            // This would typically record access patterns for resource prediction
            // For now, we'll just log the access
            logger_->debug("Recording context access: {}", context_id.c_str());
        }
        catch (const std::exception& e)
        {
            logger_->error("Error recording context access: {}", e.what());
        }
#else
        logger_->debug("Recording context access: {}", context_id.c_str());
#endif
    }
};

// Factory function to create processor instances
std::unique_ptr<Processor> createProcessor(const ProcessOptions& options)
{
    return std::make_unique<ProcessorImpl>(options);
}

// ============================================================================
// Priority Manager Implementation
// ============================================================================

PriorityManager::PriorityManager(const PriorityConfig& config) : config_(config) {}

float PriorityManager::calculateTimeDecay(time_t last_access) const
{
    time_t now                = std::time(nullptr);
    double hours_since_access = std::difftime(now, last_access) / 3600.0;

    // Apply exponential decay formula: exp(-hours * factor)
    // Use float version of exp to avoid ambiguity
    float temp = -static_cast<float>(hours_since_access) * config_.time_decay_factor;
    return sep::math::exp_safe(temp);
}

float PriorityManager::getPriorityMultiplier(PriorityTier tier) const
{
    switch (tier)
    {
        case PriorityTier::LOW:
            return multipliers_.low_multiplier;
        case PriorityTier::NORMAL:
            return multipliers_.normal_multiplier;
        case PriorityTier::HIGH:
            return multipliers_.high_multiplier;
        case PriorityTier::CRITICAL:
            return multipliers_.critical_multiplier;
        default:
            return multipliers_.normal_multiplier;
    }
}

float PriorityManager::calculateFinalScore(float base_relevance, float time_decay, PriorityTier tier) const
{
    float priority_multiplier = getPriorityMultiplier(tier);
    float final_score         = base_relevance * time_decay * priority_multiplier;

    // Clamp the score between 0 and 1
    return std::clamp(final_score, 0.0f, 1.0f);
}

void PriorityManager::updatePriority(const sep::shim::string& context_id, PriorityInfo& info)
{
#if defined(__cpp_exceptions)
    try
    {
#endif
        // Calculate time decay
        info.time_decay = calculateTimeDecay(info.last_access_time);

        // Calculate final score
        info.final_score = calculateFinalScore(info.base_score, info.time_decay, info.tier);

        spdlog::debug("Updated priority for context {}: score={}, tier={}, decay={}",
                      context_id.c_str(),
                      info.final_score,
                      static_cast<int>(info.tier),
                      info.time_decay);
#if defined(__cpp_exceptions)
    }
    catch (const std::exception& e)
    {
        spdlog::error("Error updating priority for context {}: {}", context_id.c_str(), e.what());
    }
#endif
}

void PriorityManager::recordAccess(const sep::shim::string& context_id, PriorityInfo& info)
{
#if defined(__cpp_exceptions)
    try
    {
#endif
        // Update access time and count
        info.last_access_time = std::time(nullptr);
        info.access_count++;

        // Apply usage frequency multiplier
        info.base_score *= config_.access_multiplier;

        // Clamp base score between 0 and 1
        info.base_score = std::clamp(info.base_score, 0.0f, 1.0f);

        // Update priority
        updatePriority(context_id, info);

        spdlog::debug("Recorded access for context {}: access_count={}, base_score={}",
                      context_id.c_str(),
                      info.access_count,
                      info.base_score);
#if defined(__cpp_exceptions)
    }
    catch (const std::exception& e)
    {
        spdlog::error("Error recording access for context {}: {}", context_id.c_str(), e.what());
    }
#endif
}

bool PriorityManager::shouldExpire(const PriorityInfo& info) const
{
    time_t now                = std::time(nullptr);
    double hours_since_access = std::difftime(now, info.last_access_time) / 3600.0;

    // Check if the context has been inactive for longer than the expiration period
    return hours_since_access > config_.expiration_hours;
}

bool PriorityManager::shouldRecalculatePriority(const PriorityInfo& info) const
{
    time_t now                  = std::time(nullptr);
    double minutes_since_access = std::difftime(now, info.last_access_time) / 60.0;

    // Check if it's time to recalculate priority
    return minutes_since_access > config_.recalc_interval_minutes;
}

void PriorityManager::adjustPriorityForRelationships(PriorityInfo& info, std::size_t relationship_count)
{
    // Apply relationship count weighting
    float relationship_factor = 1.0f + (relationship_count * config_.relationship_weight);

    // Adjust base score
    info.base_score *= relationship_factor;

    // Clamp base score between 0 and 1
    info.base_score = std::clamp(info.base_score, 0.0f, 1.0f);

    // Potentially upgrade tier based on relationship count
    if (relationship_count > 100 && info.tier == PriorityTier::LOW)
    {
        info.tier = PriorityTier::NORMAL;
    }
    else if (relationship_count > 500 && info.tier == PriorityTier::NORMAL)
    {
        info.tier = PriorityTier::HIGH;
    }
}

void PriorityManager::adjustPriorityForParent(PriorityInfo& info, const PriorityInfo& parent_info)
{
    // Apply parent influence
    float parent_factor = 1.0f + (parent_info.final_score * config_.parent_influence);

    // Adjust base score
    info.base_score *= parent_factor;

    // Clamp base score between 0 and 1
    info.base_score = std::clamp(info.base_score, 0.0f, 1.0f);

    // Potentially upgrade tier based on parent's tier
    if (parent_info.tier == PriorityTier::CRITICAL && info.tier != PriorityTier::CRITICAL)
    {
        info.tier = PriorityTier::HIGH;
    }
    else if (parent_info.tier == PriorityTier::HIGH && info.tier == PriorityTier::LOW)
    {
        info.tier = PriorityTier::NORMAL;
    }
}



RelationshipManager::RelationshipManager(const RelationshipConfig& config) : config_(config) {}

bool RelationshipManager::addRelationship(const std::string& source_id,
                                          const std::string& target_id,
                                          uint8_t type,
                                          float              strength,
                                          bool               bidirectional)
{
#if defined(__cpp_exceptions)
    try
    {
#endif
        // Check if relationship strength meets minimum threshold
        if (strength < config_.min_strength_threshold)
        {
            spdlog::debug(
                "Relationship strength below threshold: {} -> {}, strength={}", source_id.c_str(), target_id.c_str(), strength);
            return false;
        }

        // Check if source already has maximum relationships
        if (exceedsMaxRelationships(source_id))
        {
            spdlog::debug("Source exceeds maximum relationships: {}", source_id.c_str());

            // If auto-prune is enabled, remove weak relationships
            if (config_.auto_prune)
            {
                size_t pruned = pruneWeakRelationships(source_id);
                spdlog::debug("Pruned {} weak relationships from {}", pruned, source_id.c_str());

                // Check again after pruning
                if (exceedsMaxRelationships(source_id))
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }

        // Create relationship info
        RelationshipInfo info;
        info.target_id     = target_id;
        info.type          = type;
        info.strength      = strength;
        info.bidirectional = bidirectional;

        // Add relationship
        relationships_[source_id].push_back(info);

        // Add bidirectional relationship if requested
        if (bidirectional)
        {
            RelationshipInfo reverse_info;
            reverse_info.target_id     = source_id;
            reverse_info.type          = type;
            reverse_info.strength      = strength;
            reverse_info.bidirectional = true;

            relationships_[target_id].push_back(reverse_info);
        }

        spdlog::debug("Added relationship: {} -> {}, type={}, strength={}, bidirectional={}",
                      source_id.c_str(),
                      target_id.c_str(),
                      static_cast<int>(type),
                      strength,
                      bidirectional);

        return true;
#if defined(__cpp_exceptions)
    }
    catch (const std::exception& e)
    {
        spdlog::error("Error adding relationship: {}", e.what());
        return false;
    }
#endif
}

bool RelationshipManager::removeRelationship(const std::string& source_id, const std::string& target_id)
{
#if defined(__cpp_exceptions)
    try
    {
#endif
        // Check if source has any relationships
        if (relationships_.find(source_id) == relationships_.end())
        {
            return false;
        }

        // Find and remove the relationship
        auto& rels = relationships_[source_id];
        auto  it   = std::find_if(rels.begin(), rels.end(), [&target_id](const RelationshipInfo& info) {
            return info.target_id == target_id;
        });

        if (it == rels.end())
        {
            return false;
        }

        // Check if relationship is bidirectional
        bool bidirectional = it->bidirectional;

        // Remove the relationship
        rels.erase(it);

        // Remove bidirectional relationship if it exists
        if (bidirectional)
        {
            if (relationships_.find(target_id) != relationships_.end())
            {
                auto& target_rels = relationships_[target_id];
                auto  target_it =
                    std::find_if(target_rels.begin(), target_rels.end(), [&source_id](const RelationshipInfo& info) {
                        return info.target_id == source_id;
                    });

                if (target_it != target_rels.end())
                {
                    target_rels.erase(target_it);
                }
            }
        }

        spdlog::debug("Removed relationship: {} -> {}", source_id.c_str(), target_id.c_str());

        return true;
#if defined(__cpp_exceptions)
    }
    catch (const std::exception& e)
    {
        spdlog::error("Error removing relationship: {}", e.what());
        return false;
    }
#endif
}

std::vector<RelationshipInfo> RelationshipManager::getRelationships(const std::string& context_id) const
{
#if defined(__cpp_exceptions)
    try
    {
#endif
        auto it = relationships_.find(context_id);
        if (it != relationships_.end())
        {
            return it->second;
        }
        return {};
#if defined(__cpp_exceptions)
    }
    catch (const std::exception& e)
    {
        spdlog::error("Error getting relationships: {}", e.what());
        return {};
    }
#endif
}

std::vector<std::string> RelationshipManager::getChildren(const std::string& parent_id) const
{
#if defined(__cpp_exceptions)
    try
    {
#endif
        std::vector<std::string> children;

        auto it = relationships_.find(parent_id);
        if (it != relationships_.end())
        {
            for (const auto& rel : it->second)
            {
                if (rel.type == 1) // CHILD
                {
                    children.push_back(rel.target_id);
                }
            }
        }

        return children;
#if defined(__cpp_exceptions)
    }
    catch (const std::exception& e)
    {
        spdlog::error("Error getting children: {}", e.what());
        return {};
    }
#endif
}

std::string RelationshipManager::getParent(const std::string& child_id) const
{
#if defined(__cpp_exceptions)
    try
    {
#endif
        auto it = relationships_.find(child_id);
        if (it != relationships_.end())
        {
            for (const auto& rel : it->second)
            {
                if (rel.type == 2) // PARENT
                {
                    return rel.target_id;
                }
            }
        }

        return "";
#if defined(__cpp_exceptions)
    }
    catch (const std::exception& e)
    {
        spdlog::error("Error getting parent: {}", e.what());
        return "";
    }
#endif
}

float RelationshipManager::calculateRelationshipStrength(const std::vector<float>& embedding1,
                                                         const std::vector<float>& embedding2) const
{
#if defined(__cpp_exceptions)
    try
    {
#endif
        if (embedding1.empty() || embedding2.empty() || embedding1.size() != embedding2.size())
        {
            return 0.0f;
        }

        // Calculate dot product and norms
        float dot_product = 0.0f;
        float norm1       = 0.0f;
        float norm2       = 0.0f;

        for (size_t i = 0; i < embedding1.size(); ++i)
        {
            dot_product += embedding1[i] * embedding2[i];
            norm1 += embedding1[i] * embedding1[i];
            norm2 += embedding2[i] * embedding2[i];
        }

        // Geodesic distance on unit sphere (deterministic manifold traversal)
        if (norm1 > 0.0f && norm2 > 0.0f)
        {
            float cos_theta        = dot_product / (sep::math::sqrt_safe(norm1) * sep::math::sqrt_safe(norm2));
            cos_theta              = std::clamp(cos_theta, -1.0f, 1.0f);
            const float pi         = 3.14159265358979323846f;
            float       distance   = std::acos(cos_theta);
            float       similarity = 1.0f - (distance / pi);
            return std::clamp(similarity, 0.0f, 1.0f);
        }

        return 0.0f;
#if defined(__cpp_exceptions)
    }
    catch (const std::exception& e)
    {
        spdlog::error("Error calculating relationship strength: {}", e.what());
        return 0.0f;
    }
#endif
}

size_t RelationshipManager::pruneWeakRelationships(const std::string& context_id)
{
#if defined(__cpp_exceptions)
    try
    {
#endif
        auto it = relationships_.find(context_id);
        if (it == relationships_.end())
        {
            return 0;
        }

        auto&  rels         = it->second;
        size_t initial_size = rels.size();

        // Sort relationships by strength (ascending)
        std::sort(rels.begin(), rels.end(), [](const RelationshipInfo& a, const RelationshipInfo& b) {
            return a.strength < b.strength;
        });

        // Find the first relationship with strength >= threshold
        auto threshold_it = std::find_if(rels.begin(), rels.end(), [this](const RelationshipInfo& info) {
            return info.strength >= config_.min_strength_threshold;
        });

        // Remove all relationships below threshold
        rels.erase(rels.begin(), threshold_it);

        size_t pruned = initial_size - rels.size();
        spdlog::debug("Pruned {} weak relationships from {}", pruned, context_id.c_str());

        return pruned;
#if defined(__cpp_exceptions)
    }
    catch (const std::exception& e)
    {
        spdlog::error("Error pruning weak relationships: {}", e.what());
        return 0;
    }
#endif
}

bool RelationshipManager::exceedsMaxRelationships(const std::string& context_id) const
{
#if defined(__cpp_exceptions)
    try
    {
#endif
        auto it = relationships_.find(context_id);
        if (it == relationships_.end())
        {
            return false;
        }

        return it->second.size() >= config_.max_relationships;
#if defined(__cpp_exceptions)
    }
    catch (const std::exception& e)
    {
        spdlog::error("Error checking max relationships: {}", e.what());
        return false;
    }
#endif
}

size_t RelationshipManager::getRelationshipCount(const std::string& context_id) const
{
#if defined(__cpp_exceptions)
    try
    {
#endif
        auto it = relationships_.find(context_id);
        if (it == relationships_.end())
        {
            return 0;
        }

        return it->second.size();
#if defined(__cpp_exceptions)
    }
    catch (const std::exception& e)
    {
        spdlog::error("Error getting relationship count: {}", e.what());
        return 0;
    }
#endif
}

float RelationshipManager::storeCosineSimilarity(const std::string&        source_id,
                                                 const std::string&        target_id,
                                                 const std::vector<float>& emb_a,
                                                 const std::vector<float>& emb_b)
{
    float sim                             = cosineSimilarity(emb_a, emb_b);
    similarity_map_[source_id][target_id] = sim;
    return sim;
}

bool RelationshipManager::storePatternMatch(const std::string& source_id,
                                            const std::string& target_id,
                                            const std::string& pattern,
                                            const std::string& text)
{
    bool match                         = simplePatternMatch(pattern, text);
    pattern_map_[source_id][target_id] = match;
    return match;
}

// Continuing in the same namespace sep::context

namespace {
constexpr size_t MIN_PATTERNS_FOR_PREDICTION = 5;
constexpr double ANOMALY_THRESHOLD           = 3.0;

struct Prediction
{
    size_t estimated_memory{0};
    float estimated_cpu_usage{0.0f};
    float estimated_gpu_usage{0.0f};
    size_t optimal_batch_size{0};
    double expected_processing_time{0.0};
    float  confidence_score{0.0f};
};

#ifdef __linux__
struct CpuTimes
{
    unsigned long long user{0};
    unsigned long long nice{0};
    unsigned long long system{0};
    unsigned long long idle{0};
    unsigned long long iowait{0};
    unsigned long long irq{0};
    unsigned long long softirq{0};
    unsigned long long steal{0};
};

static CpuTimes readCpuTimes()
{
    std::ifstream file("/proc/stat");
    CpuTimes      times{};
    if (file.is_open())
    {
        std::string cpu;
        file >> cpu >> times.user >> times.nice >> times.system >> times.idle >> times.iowait >> times.irq
            >> times.softirq >> times.steal;
    }
    return times;
}
#endif

static float queryCpuUsage()
{
#ifdef __linux__
    static CpuTimes prev = readCpuTimes();
    CpuTimes        cur  = readCpuTimes();

    unsigned long long prevIdle = prev.idle + prev.iowait;
    unsigned long long idle     = cur.idle + cur.iowait;

    unsigned long long prevNonIdle = prev.user + prev.nice + prev.system + prev.irq + prev.softirq + prev.steal;
    unsigned long long nonIdle     = cur.user + cur.nice + cur.system + cur.irq + cur.softirq + cur.steal;

    unsigned long long prevTotal = prevIdle + prevNonIdle;
    unsigned long long total     = idle + nonIdle;

    double totald = static_cast<double>(total - prevTotal);
    double idled  = static_cast<double>(idle - prevIdle);

    prev = cur;

    if (totald <= 0.0)
        return 0.0f;

    return static_cast<float>((totald - idled) / totald);
#else
    return 0.0f;
#endif
}

static std::size_t queryTotalMemory()
{
#ifdef __linux__
    struct sysinfo info;
    if (sysinfo(&info) == 0)
    {
        return static_cast<std::size_t>(info.totalram) * info.mem_unit;
    }
#endif
    return 0;
}

static std::size_t queryFreeMemory()
{
#ifdef __linux__
    struct sysinfo info;
    if (sysinfo(&info) == 0)
    {
        return static_cast<std::size_t>(info.freeram) * info.mem_unit;
    }
#endif
    return 0;
}
}  // namespace

class ResourcePredictorImpl final : public ResourcePredictor
{
public:
    explicit ResourcePredictorImpl(size_t history_size = 1000)
        : max_history_size_(history_size)
        , avg_memory_usage_(0)
        , avg_processing_time_(0)
        , avg_batch_size_(0)
        , avg_gpu_utilization_(0)
        , avg_cpu_utilization_(0)
    {}

    ResourcePredictorImpl(const ResourcePredictorImpl&)                = delete;
    ResourcePredictorImpl& operator=(const ResourcePredictorImpl&)     = delete;
    ResourcePredictorImpl(ResourcePredictorImpl&&) noexcept            = default;
    ResourcePredictorImpl& operator=(ResourcePredictorImpl&&) noexcept = default;

    ResourcePrediction predictResourceNeeds(const Batch& batch) override
    {
        auto pred =
            predictResourceNeeds(batch.contexts.size(), batch.contexts.empty() ? std::string("unknown") : std::string(batch.contexts[0].type.c_str()));

        ResourcePrediction result;
        result.estimated_memory         = pred.estimated_memory;
        result.estimated_cpu_usage      = queryCpuUsage();
        result.estimated_gpu_usage      = 0.6f;
        result.optimal_batch_size       = pred.optimal_batch_size;
        result.expected_processing_time = pred.expected_processing_time;
        result.confidence_score         = pred.confidence_score;
        return result;
    }

    void recordBatchProcessing(const Batch& batch, size_t memory_used, double processing_time) override
    {
        UsagePattern pattern;
        pattern.memory_used     = memory_used;
        pattern.batch_size      = batch.contexts.size();
        pattern.processing_time = processing_time;
        pattern.cpu_utilization = queryCpuUsage();
        pattern.gpu_utilization = 0.6f;

        recordUsagePattern(pattern);
    }

    void recordUsagePattern(const UsagePattern& pattern) override
    {
        if (!isPatternAnomaly(pattern))
        {
            usage_history_.push_back(pattern);
            updateMovingAverages(pattern);
            pruneOldPatterns();
        }
    }

    ResourceState getCurrentState() const override
    {
        ResourceState state;
        state.total_memory    = queryTotalMemory();
        state.free_memory     = queryFreeMemory();
        state.used_memory     = state.total_memory > state.free_memory ? state.total_memory - state.free_memory : 0;
        state.active_batches  = usage_history_.size();
        state.gpu_utilization = avg_gpu_utilization_;
        state.cpu_utilization = queryCpuUsage();
        return state;
    }

    ResourceMetrics getResourceMetrics() const override
    {
        ResourceMetrics metrics;
        metrics.peak_memory_usage       = static_cast<size_t>(avg_memory_usage_ * 1.5);
        metrics.average_memory_usage    = static_cast<size_t>(avg_memory_usage_);
        metrics.average_processing_time = avg_processing_time_;
        metrics.total_batches_processed = usage_history_.size();
        metrics.resource_efficiency     = calculateResourceEfficiency();
        return metrics;
    }

    float calculateResourceEfficiency() const override
    {
        if (usage_history_.empty())
        {
            return 0.0f;
        }
        float mem_eff = 1.0f;
        if (max_memory_ != SIZE_MAX && max_memory_ > 0)
        {
            mem_eff = 1.0f - static_cast<float>(avg_memory_usage_ / static_cast<double>(max_memory_));
        }

        float cpu_eff = 1.0f;
        if (max_cpu_usage_ > 0.0f)
        {
            cpu_eff = 1.0f - (avg_cpu_utilization_ / max_cpu_usage_);
        }

        float gpu_eff = 1.0f;
        if (max_gpu_usage_ > 0.0f)
        {
            gpu_eff = 1.0f - (avg_gpu_utilization_ / max_gpu_usage_);
        }

        float efficiency = (mem_eff + cpu_eff + gpu_eff) / 3.0f;
        return std::clamp(efficiency, 0.0f, 1.0f);
    }

    ::sep::shim::vector<std::size_t> suggestBatchSizes(std::size_t target_throughput) const override
    {
        ::sep::shim::vector<std::size_t> sizes;

        double mem_per_context = 0.0;
        if (avg_batch_size_ > 0.0)
        {
            mem_per_context = avg_memory_usage_ / avg_batch_size_;
        }

        std::size_t historical_size = avg_batch_size_ > 1.0 ? static_cast<std::size_t>(avg_batch_size_) : 32;

        std::size_t max_size = historical_size;
        if (mem_per_context > 0.0 && max_memory_ != SIZE_MAX && max_memory_ > 0)
        {
            max_size = std::min(max_size, static_cast<std::size_t>(max_memory_ / mem_per_context));
        }
        max_size = std::max<std::size_t>(1, max_size);

        while (target_throughput > 0)
        {
            std::size_t batch = std::min(target_throughput, max_size);
            sizes.push_back(batch);
            target_throughput -= batch;
        }

        return sizes;
    }

    void updateResourceLimits(size_t max_memory, float max_cpu_usage, float max_gpu_usage) override
    {
        max_memory_    = max_memory;
        max_cpu_usage_ = max_cpu_usage;
        max_gpu_usage_ = max_gpu_usage;
    }

    void resetModel() override
    {
        usage_history_.clear();
        avg_memory_usage_    = 0;
        avg_processing_time_ = 0;
        avg_batch_size_      = 0;
        avg_gpu_utilization_ = 0;
        avg_cpu_utilization_ = 0;
    }

private:
    Prediction predictResourceNeeds(size_t context_count, const std::string& /*context_type*/)
    {
        auto relevant_patterns = findRelevantPatterns("");

        if (relevant_patterns.size() < MIN_PATTERNS_FOR_PREDICTION)
        {
            double mem_per_context  = avg_batch_size_ > 0.0 ? avg_memory_usage_ / avg_batch_size_ : 0.0;
            double time_per_context = avg_batch_size_ > 0.0 ? avg_processing_time_ / avg_batch_size_ : 0.0;

            Prediction result;
            result.estimated_memory         = static_cast<size_t>(mem_per_context * context_count * 1.5);
            result.optimal_batch_size       = std::min(context_count,
                                                 avg_batch_size_ > 1.0 ? static_cast<std::size_t>(avg_batch_size_)
                                                                             : static_cast<std::size_t>(32));
            result.expected_processing_time = time_per_context * context_count;
            result.confidence_score         = 0.5f;
            return result;
        }

        return generatePrediction(relevant_patterns, context_count);
    }

    Prediction generatePrediction(const std::vector<UsagePattern>& patterns, std::size_t context_count) const
    {
        if (patterns.empty())
        {
            return Prediction{};
        }

        double total_memory = 0;
        double total_time   = 0;
        double total_batch  = 0;
        size_t count        = patterns.size();

        for (const auto& pattern : patterns)
        {
            total_memory += static_cast<double>(pattern.memory_used);
            total_time += pattern.processing_time;
            total_batch += static_cast<double>(pattern.batch_size);
        }

        double avg_memory = total_memory / static_cast<double>(count);
        double avg_time   = total_time / static_cast<double>(count);
        double avg_batch  = total_batch / static_cast<double>(count);

        double mem_per_context  = avg_batch > 0.0 ? avg_memory / avg_batch : 0.0;
        double time_per_context = avg_batch > 0.0 ? avg_time / avg_batch : 0.0;

        Prediction result;
        result.estimated_memory         = static_cast<size_t>(mem_per_context * context_count * 1.1);
        result.optimal_batch_size       = static_cast<std::size_t>(avg_batch);
        result.expected_processing_time = time_per_context * context_count;
        result.confidence_score         = calculateConfidenceScore(patterns);
        return result;
    }

    std::vector<UsagePattern> findRelevantPatterns(const std::string& /*context_type*/) const
    {
        return std::vector<UsagePattern>(usage_history_.begin(), usage_history_.end());
    }

    float calculateConfidenceScore(const std::vector<UsagePattern>& patterns) const
    {
        if (patterns.size() < MIN_PATTERNS_FOR_PREDICTION)
        {
            return 0.5f;
        }

        double mean_memory =
            std::accumulate(patterns.begin(),
                            patterns.end(),
                            0.0,
                            [](double sum, const auto& p) { return sum + static_cast<double>(p.memory_used); })
            / static_cast<double>(patterns.size());

        double variance = std::accumulate(patterns.begin(),
                                          patterns.end(),
                                          0.0,
                                          [mean_memory](double sum, const auto& p) {
                                              double diff = static_cast<double>(p.memory_used) - mean_memory;
                                              return sum + (diff * diff);
                                          })
                          / static_cast<double>(patterns.size());

        // Use float version of sqrt to avoid ambiguity
        // Removed unused variable: float temp_var = static_cast<float>(variance);
        double std_dev                  = sep::math::sqrt_safe(variance);
        double coefficient_of_variation = std_dev / mean_memory;

        float confidence = static_cast<float>(1.0 / (1.0 + coefficient_of_variation));
        return std::clamp(confidence, 0.0f, 1.0f);
    }

    void updateMovingAverages(const UsagePattern& pattern)
    {
        constexpr double alpha   = 0.1;
        const float      alpha_f = static_cast<float>(alpha);

        avg_memory_usage_    = (1.0 - alpha) * avg_memory_usage_ + alpha * static_cast<double>(pattern.memory_used);
        avg_processing_time_ = (1.0 - alpha) * avg_processing_time_ + alpha * pattern.processing_time;
        avg_batch_size_      = (1.0 - alpha) * avg_batch_size_ + alpha * static_cast<double>(pattern.batch_size);
        avg_gpu_utilization_ = (1.0f - alpha_f) * avg_gpu_utilization_ + alpha_f * pattern.gpu_utilization;
        avg_cpu_utilization_ = (1.0f - alpha_f) * avg_cpu_utilization_ + alpha_f * pattern.cpu_utilization;
    }

    void pruneOldPatterns()
    {
        while (usage_history_.size() > max_history_size_)
        {
            usage_history_.pop_front();
        }
    }

    bool isPatternAnomaly(const UsagePattern& pattern) const
    {
        if (usage_history_.size() < MIN_PATTERNS_FOR_PREDICTION)
        {
            return false;
        }

        double mem_mean  = 0.0;
        double time_mean = 0.0;
        for (const auto& p : usage_history_)
        {
            mem_mean += static_cast<double>(p.memory_used);
            time_mean += p.processing_time;
        }
        mem_mean /= static_cast<double>(usage_history_.size());
        time_mean /= static_cast<double>(usage_history_.size());

        double mem_var  = 0.0;
        double time_var = 0.0;
        for (const auto& p : usage_history_)
        {
            double dm = static_cast<double>(p.memory_used) - mem_mean;
            mem_var += dm * dm;
            double dt = p.processing_time - time_mean;
            time_var += dt * dt;
        }
        mem_var /= static_cast<double>(usage_history_.size());
        time_var /= static_cast<double>(usage_history_.size());

        // Use float versions of sqrt to avoid ambiguity
        float  mem_var_f  = static_cast<float>(mem_var);
        float  time_var_f = static_cast<float>(time_var);
        float  mem_std    = sep::math::sqrt_safe(mem_var_f);
        float  time_std   = sep::math::sqrt_safe(time_var_f);

        double mem_z  = mem_std > 0.0f ? std::fabs(static_cast<double>(pattern.memory_used) - mem_mean) / static_cast<double>(mem_std) : 0.0;
        double time_z = time_std > 0.0f ? std::fabs(pattern.processing_time - time_mean) / static_cast<double>(time_std) : 0.0;

        return mem_z > ANOMALY_THRESHOLD || time_z > ANOMALY_THRESHOLD;
    }

    std::deque<UsagePattern> usage_history_;
    size_t                   max_history_size_;
    double                   avg_memory_usage_;
    double                   avg_processing_time_;
    double                   avg_batch_size_;
    float                    avg_gpu_utilization_;
    float                    avg_cpu_utilization_;
    size_t                   max_memory_{SIZE_MAX};
    float                    max_cpu_usage_{1.0f};
    float                    max_gpu_usage_{1.0f};
};

// Factory function
std::unique_ptr<ResourcePredictor> createResourcePredictor(size_t history_size)
{
    return std::unique_ptr<ResourcePredictor>(std::make_unique<ResourcePredictorImpl>(history_size));
}

}  // namespace sep::context
