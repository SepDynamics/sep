/**
 * @file crow_adapter.cpp
 * @brief Adapter for integrating the SEP Engine API with the Crow web framework
 *
 * This file provides the necessary adapters and route handlers to expose the
 * SEP Engine API via HTTP endpoints using the Crow web framework.
 */

#include "crow/common.h"
#include "crow/http_request.h"
#include "crow/http_response.h"

// Ensure C string functions are available for any API headers using them
#include <cstring>

// Include our API headers
#include "api/crow_adapter.h"
#include "api/json_helpers.h"
#include "api/types.h"
#include "api/sep_engine.h"
#include "api/server.h" // Include server header
#include "core/types.h"  // For sep::config::APIConfig
#include "core/manager.h" // For sep::config::ConfigManager
#include "memory/memory_tier_manager.hpp"

// Include standard headers
#include <memory>
#include <string>
#include <nlohmann/json.hpp>

namespace sep::api {

// -----------------------------
// Crow request/response adapters
// -----------------------------

CrowRequestAdapter::CrowRequestAdapter(::crow::request &req) : req_(req) {
    method_str_ = ::crow::method_name(req.method);
}

std::string CrowRequestAdapter::url() const { return std::string(req_.url.c_str()); }

std::string CrowRequestAdapter::method() const { return method_str_; }

std::string CrowRequestAdapter::body() const { return std::string(req_.body.c_str()); }

CrowResponseAdapter::CrowResponseAdapter(::crow::response &res) : res_(res) {}

void CrowResponseAdapter::setCode(int code) { res_.code = code; }

int CrowResponseAdapter::getCode() const { return res_.code; }

void CrowResponseAdapter::setBody(const std::string &body) { res_.body = body.c_str(); }

void CrowResponseAdapter::end() { res_.end(); }

std::string CrowResponseAdapter::getBody() const { return std::string(res_.body.c_str()); }

std::unique_ptr<HttpResponse> makeResponse(::crow::response &res) {
    return std::make_unique<CrowResponseAdapter>(res);
}

std::unique_ptr<HttpRequest> makeRequest(::crow::request &req) {
    return std::make_unique<CrowRequestAdapter>(req);
}

// Define API_PREFIX here as it's used in setup_routes
#ifndef API_PREFIX
#define API_PREFIX "/api/v1" // Example prefix
#endif

// Legacy route setup was replaced by SEPApiServer::setup_routes.
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

}  // namespace sep::api
