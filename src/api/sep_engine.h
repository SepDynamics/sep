/**
 * @file sep_engine.h
 * @brief Main API interface for the SEP Engine
 *
 * This file defines the SepEngine class that serves as the main API interface,
 * exposing methods for context processing, pattern recognition, and memory management.
 */

#pragma once

#include <atomic>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>

#include "api/types.h"
#include "engine/internal/types.h"
#include "pattern/pattern_processor.hpp"
#include "quantum/pattern_processor.hpp"
#include "quantum/processor.h"
#include "quantum/quantum_processor.h"
#include "quantum/relationship.h"
#include "quantum/types.h"

namespace sep::api {

/**
 * @brief Main API interface for the SEP Engine
 *
 * This class provides a unified interface to the SEP Engine, exposing methods
 * for context processing, pattern recognition, and memory management.
 * It follows the Singleton pattern to ensure a single instance is used throughout
 * the application.
 */
class SepEngine
{
public:
    /**
     * @brief Get the singleton instance of the SepEngine
     *
     * @return Reference to the singleton instance
     */
    static SepEngine& getInstance();

    /**
     * @brief Initialize the SEP Engine
     *
     * This method initializes all components of the SEP Engine, including
     * the context processor, pattern evolution, and memory tier manager.
     *
     * @param config API configuration data
     * @return JSON response with initialization results
     */
    nlohmann::json initialize(const config::SystemConfig& config);

    /**
     * @brief Shutdown the SEP Engine
     *
     * @return JSON response with shutdown results
     */
    nlohmann::json shutdown();

    /**
     * @brief Process patterns through the SEP Engine
     *
     * @param request_data JSON request data containing pattern information
     * @return JSON response with processing results
     */
    nlohmann::json processPatterns(const nlohmann::json& request_data);

    /**
     * @brief Process a batch of patterns
     *
     * @param request_data JSON request data containing batch information
     * @return JSON response with batch processing results
     */
    nlohmann::json processBatch(const nlohmann::json& request_data);

    /**
     * @brief Validate context data
     *
     * @param request_data JSON request data containing context information
     * @return JSON response with validation results
     */
    nlohmann::json validateContexts(const nlohmann::json& request_data);

    /**
     * @brief Retrieve recent pattern history metrics
     */
    nlohmann::json getPatternHistory(const nlohmann::json& request_data);

    /**
     * @brief Extract embeddings from data
     *
     * @param request_data JSON request data containing data to embed
     * @return JSON response with embedding results
     */
    nlohmann::json extractEmbeddings(const nlohmann::json& request_data);

    /**
     * @brief Calculate similarity between embeddings
     *
     * @param request_data JSON request data containing embeddings to compare
     * @return JSON response with similarity results
     */
    nlohmann::json calculateSimilarity(const nlohmann::json& request_data);

    /**
     * @brief Blend contexts together
     *
     * @param request_data JSON request data containing contexts to blend
     * @return JSON response with blending results
     */
    nlohmann::json blendContexts(const nlohmann::json& request_data);

    /**
     * @brief Get the health status of the SEP Engine
     *
     * This method returns the health status of the SEP Engine, including
     * component status, resource usage, and error information.
     *
     * @return JSON response with health status
     */
    nlohmann::json getHealthStatus();

    static nlohmann::json getMemoryMetrics();
    static nlohmann::json getConfig(const sep::config::SystemConfig& config);
    static nlohmann::json makeErrorResponse(ErrorCode code, const std::string& message);
    static bool
    validateFields(const nlohmann::json& data, const std::vector<std::string>& fields, nlohmann::json& error);

    /**
     * @brief Generate deterministic ID
     */
    static std::string generateId(const std::string& prefix);

private:
    /**
     * @brief Private constructor to enforce Singleton pattern
     */
    SepEngine();

    /**
     * @brief Private destructor to enforce Singleton pattern
     */
    ~SepEngine();

    /**
     * @brief Deleted copy constructor to enforce Singleton pattern
     */
    SepEngine(const SepEngine&) = delete;

    /**
     * @brief Deleted assignment operator to enforce Singleton pattern
     */
    SepEngine& operator=(const SepEngine&) = delete;

    struct Impl;
    std::unique_ptr<Impl> impl_;

    /**
     * @brief Get metrics from HealthMetrics struct
     */
    nlohmann::json getMetrics(const HealthMetrics& metrics);

    /**
     * @brief ID counter for deterministic identifiers
     */
    static std::atomic<uint64_t> id_counter_;
};

}  // namespace sep::api
