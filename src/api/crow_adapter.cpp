/**
 * @file crow_adapter.cpp
 * @brief Adapter for integrating the SEP Engine API with the Crow web framework
 *
 * This file provides the necessary adapters and route handlers to expose the
 * SEP Engine API via HTTP endpoints using the Crow web framework.
 */

// CROW_DISABLE_RTTI is defined globally via CMake

// First include our fixed isolation headers to avoid conflicts

#include "crow/common.h"
#include "crow/http_request.h"
#include "crow/http_response.h"

// Include standard headers
#include <memory>
#include <string>
#include <nlohmann/json.hpp>

// Include our API headers
#include "api/crow_adapter.h"
#include "api/json_helpers.h"
#include "api/types.h"
#include "api/sep_engine.h"
#include "api/server.h" // Include server header
#include "memory/memory_tier_manager.hpp"

namespace sep::api {

// -----------------------------
// Crow request/response adapters
// -----------------------------

CrowRequestAdapter::CrowRequestAdapter(::crow::request &req) : req_(req) {
    method_str_ = ::crow::method_name(req.method);
}

std::string CrowRequestAdapter::url() const { return std::string(req_.url); }

std::string CrowRequestAdapter::method() const { return method_str_; }

std::string CrowRequestAdapter::body() const { return std::string(req_.body); }

CrowResponseAdapter::CrowResponseAdapter(::crow::response &res) : res_(res) {}

void CrowResponseAdapter::setCode(int code) { res_.code = code; }

int CrowResponseAdapter::getCode() const { return res_.code; }

void CrowResponseAdapter::setBody(const std::string &body) { res_.body = body; }

void CrowResponseAdapter::end() { res_.end(); }

std::string CrowResponseAdapter::getBody() const { return std::string(res_.body); }

std::unique_ptr<HttpResponse> makeResponse(::crow::response &res) {
    return std::make_unique<CrowResponseAdapter>(res);
}

std::unique_ptr<HttpRequest> makeRequest(::crow::request &req) {
    return std::make_unique<CrowRequestAdapter>(req);
}

#define API_PREFIX "/api/v1"

/**
 * @brief Setup the SEP API routes in a Crow application
 *
 * This function integrates the SEP Engine API with a Crow web application.
 * It sets up the following endpoints:
 * - POST /api/v1/context/process - Process and validate context
 * - POST /api/v1/context/relationships - Manage context relationships
 * - POST /api/v1/pattern/analyze - Analyze pattern stability and coherence
 * - POST /api/v1/pattern/evolve - Evolve patterns through state transitions
 * - POST /api/v1/patterns/history - Get pattern evolution history
 * - POST /api/v1/memory/query - Query memory tiers for patterns
 * - GET /api/v1/health - Get the health status of the SEP Engine
 *
 * @param app The Crow application instance
 */
void setupSepApiRoutes(::crow::crow<>* app)
{
    // Get singleton engine instance and initialize
    auto&                  engine = sep::api::SepEngine::getInstance();
    sep::config::APIConfig config{};
    engine.initialize(config);

    // Safety check for null pointer
    if (!app) {
        // Return early if app is null to avoid null pointer dereference
        return;
    }

    using json_t = nlohmann::json;
    
    // Common helper function to create a consistent JSON response
    auto makeJsonResponse = [](const json_t& data, unsigned int status_code = 200) {
        ::crow::response res;
        res.body = data.dump();
        res.code = status_code;
        res.set_header("Content-Type", "application/json");
        return res;
    };
    
    // Common error handler for exceptions
    auto handleApiError = [&makeJsonResponse](const std::exception& e) {
        json_t error = {
            {"error", e.what()},
            {"status", "error"}
        };
        return makeJsonResponse(error, 400);
    };
    
    // Route: Process and validate context
    app->route_dynamic(API_PREFIX "/context/process")
        .methods(::crow::HTTPMethod::POST)
        ([&engine, makeJsonResponse, handleApiError](const ::crow::request& req) {
            try {
                auto body = parse_json(std::string(req.body));
                auto result = engine.validateContexts(body);
                return makeJsonResponse(result);
            } catch (const std::exception& e) {
                return handleApiError(e);
            }
        });

    // Route: Manage context relationships
    app->route_dynamic(API_PREFIX "/context/relationships")
        .methods(::crow::HTTPMethod::POST)
        ([&engine, makeJsonResponse, handleApiError](const ::crow::request& req) {
            try {
                auto body = parse_json(std::string(req.body));
                auto result = engine.blendContexts(body);
                return makeJsonResponse(result);
            } catch (const std::exception& e) {
                return handleApiError(e);
            }
        });

    // Route: Analyze pattern stability and coherence
    app->route_dynamic(API_PREFIX "/pattern/analyze")
        .methods(::crow::HTTPMethod::POST)
        ([&engine, makeJsonResponse, handleApiError](const ::crow::request& req) {
            try {
                auto body = parse_json(std::string(req.body));
                auto result = engine.calculateSimilarity(body);
                return makeJsonResponse(result);
            } catch (const std::exception& e) {
                return handleApiError(e);
            }
        });

    // Route: Evolve patterns through state transitions
    app->route_dynamic(API_PREFIX "/pattern/evolve")
        .methods(::crow::HTTPMethod::POST)
        ([&engine, makeJsonResponse, handleApiError](const ::crow::request& req) {
            try {
                auto body = parse_json(std::string(req.body));
                auto result = engine.processPatterns(body);
                return makeJsonResponse(result);
            } catch (const std::exception& e) {
                return handleApiError(e);
            }
        });

    // Route: Get pattern evolution history
    app->route_dynamic(API_PREFIX "/patterns/history")
        .methods(::crow::HTTPMethod::POST)
        ([&engine, makeJsonResponse, handleApiError](const ::crow::request& req) {
            try {
                auto body = parse_json(std::string(req.body));
                auto result = engine.getPatternHistory(body);
                return makeJsonResponse(result);
            } catch (const std::exception& e) {
                return handleApiError(e);
            }
        });

    // Route: Query memory tiers for patterns
    app->route_dynamic(API_PREFIX "/memory/query")
        .methods(::crow::HTTPMethod::POST)
        ([&engine, makeJsonResponse, handleApiError](const ::crow::request& req) {
            try {
                auto body = parse_json(std::string(req.body));
                auto result = engine.processBatch(body);
                return makeJsonResponse(result);
            } catch (const std::exception& e) {
                return handleApiError(e);
            }
        });

    // Route: Get health status of the SEP Engine
    app->route_dynamic(API_PREFIX "/health")
        .methods(::crow::HTTPMethod::GET)
        ([&engine, makeJsonResponse, handleApiError]() {
            try {
                auto result = engine.getHealthStatus();
                return makeJsonResponse(result);
            } catch (const std::exception& e) {
                return handleApiError(e);
            }
        });
}

}  // namespace sep::api
