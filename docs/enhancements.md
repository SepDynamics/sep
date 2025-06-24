// include/compat/shim.h - Enhanced version
// Add these methods to the existing shim::string class

namespace sep {
namespace shim {

  class string {
  private:
    char* data_;
    size_t size_;
    size_t capacity_;
    
  public:
    // ... existing constructors and methods ...
    
    // ADD THESE NEW METHODS FOR COMPATIBILITY:
    
    // Iterator support for nlohmann::json
    using value_type = char;
    using size_type = size_t;
    using const_iterator = const char*;
    using iterator = char*;
    
    const_iterator begin() const { return data_; }
    const_iterator end() const { return data_ ? data_ + size_ : nullptr; }
    iterator begin() { return data_; }
    iterator end() { return data_ ? data_ + size_ : nullptr; }
    
    // Data access
    const char* data() const { return c_str(); }
    size_t length() const { return size_; }
    
    // For JSON parsing - provide data() and size() consistently
    const char* c_str() const { return data_ ? data_ : ""; }
    size_t size() const { return size_; }
    
    // Ensure empty() is defined
    bool empty() const { return size_ == 0; }
    
    // Character access (needed by some parsers)
    char operator[](size_t pos) const {
        return (pos < size_) ? data_[pos] : '\0';
    }
    
    char& operator[](size_t pos) {
        return data_[pos];
    }
    
    // Find operation (basic implementation)
    size_t find(char c, size_t pos = 0) const {
        if (!data_ || pos >= size_) return npos;
        for (size_t i = pos; i < size_; ++i) {
            if (data_[i] == c) return i;
        }
        return npos;
    }
    
    static const size_t npos = static_cast<size_t>(-1);
    
    // Substr operation
    string substr(size_t pos = 0, size_t count = npos) const {
        if (pos >= size_) return string();
        size_t rcount = (count == npos || pos + count > size_) ? 
                        size_ - pos : count;
        char* temp = static_cast<char*>(malloc(rcount + 1));
        if (temp) {
            memcpy(temp, data_ + pos, rcount);
            temp[rcount] = '\0';
            string result(temp);
            free(temp);
            return result;
        }
        return string();
    }
    
    // Append operations
    string& append(const char* s) {
        if (s) {
            size_t slen = strlen(s);
            if (size_ + slen + 1 > capacity_) {
                size_t new_cap = (size_ + slen + 1) * 2;
                char* new_data = static_cast<char*>(realloc(data_, new_cap));
                if (new_data) {
                    data_ = new_data;
                    capacity_ = new_cap;
                } else {
                    return *this; // Failed to allocate
                }
            }
            memcpy(data_ + size_, s, slen + 1);
            size_ += slen;
        }
        return *this;
    }
    
    string& operator+=(const char* s) {
        return append(s);
    }
    
    string& operator+=(const string& other) {
        return append(other.c_str());
    }
    
    // Comparison operators
    bool operator==(const string& other) const {
        if (size_ != other.size_) return false;
        if (!data_ && !other.data_) return true;
        if (!data_ || !other.data_) return false;
        return memcmp(data_, other.data_, size_) == 0;
    }
    
    bool operator!=(const string& other) const {
        return !(*this == other);
    }
    
    bool operator<(const string& other) const {
        size_t min_size = (size_ < other.size_) ? size_ : other.size_;
        if (data_ && other.data_) {
            int cmp = memcmp(data_, other.data_, min_size);
            if (cmp != 0) return cmp < 0;
        }
        return size_ < other.size_;
    }
  };
  
  // String concatenation
  inline string operator+(const string& lhs, const string& rhs) {
      string result(lhs);
      result += rhs;
      return result;
  }
  
  inline string operator+(const string& lhs, const char* rhs) {
      string result(lhs);
      result += rhs;
      return result;
  }
  
  inline string operator+(const char* lhs, const string& rhs) {
      string result(lhs);
      result += rhs;
      return result;
  }

} // namespace shim
} // namespace sep

// Add specialization for std::hash if needed
namespace std {
    template<>
    struct hash<sep::shim::string> {
        size_t operator()(const sep::shim::string& s) const {
            size_t h = 0;
            const char* data = s.data();
            size_t len = s.size();
            for (size_t i = 0; i < len; ++i) {
                h = h * 31 + static_cast<size_t>(data[i]);
            }
            return h;
        }
    };
} // namespace std

---

// include/api/json_helpers.h
#pragma once

#include "compat/shim.h"
#include <nlohmann/json.hpp>
#include <string>

namespace sep {
namespace api {

/**
 * @brief Parse JSON from a shim::string
 * 
 * This helper converts shim::string to std::string before parsing
 * to ensure compatibility with nlohmann::json
 */
inline nlohmann::json parse_json(const sep::shim::string& str) {
    // Convert to std::string first for guaranteed compatibility
    std::string std_str(str.c_str(), str.size());
    return nlohmann::json::parse(std_str);
}

/**
 * @brief Parse JSON from std::string
 */
inline nlohmann::json parse_json(const std::string& str) {
    return nlohmann::json::parse(str);
}

/**
 * @brief Parse JSON from C string
 */
inline nlohmann::json parse_json(const char* str) {
    return nlohmann::json::parse(str);
}

/**
 * @brief Safe JSON parsing with error handling
 */
template<typename StringType>
inline nlohmann::json parse_json_safe(const StringType& str, 
                                     nlohmann::json default_value = nlohmann::json{}) {
    try {
        return parse_json(str);
    } catch (const nlohmann::json::parse_error&) {
        return default_value;
    }
}

/**
 * @brief Convert various string types to std::string for logging
 */
inline std::string to_std_string(const sep::shim::string& str) {
    return std::string(str.c_str(), str.size());
}

inline std::string to_std_string(const std::string& str) {
    return str;
}

inline std::string to_std_string(const char* str) {
    return str ? std::string(str) : std::string();
}

} // namespace api
} // namespace sep

---

// src/api/crow_adapter.cpp
/**
 * @file crow_adapter.cpp
 * @brief Adapter for integrating the SEP Engine API with the Crow web framework
 */

// Define CROW_DISABLE_RTTI first since we're using with CUDA
#define CROW_DISABLE_RTTI 1

// Include API headers first
#include "api/crow_adapter.h"
#include "api/json_helpers.h"  // Our JSON compatibility helpers
#include "api/types.h"
#include "api/sep_engine.h"

// Include crow headers
#include "crow/crow_isolation.h"
#include "crow/common.h"
#include "crow/http_request.h"
#include "crow/http_response.h"

// Include standard headers
#include <memory>
#include <string>

namespace sep::api {

// -----------------------------
// Crow request/response adapters
// -----------------------------

CrowRequestAdapter::CrowRequestAdapter(::crow::request &req) : req_(req) {
    method_str_ = ::crow::method_name(req.method);
}

// Return by value to avoid reference to temporary
std::string CrowRequestAdapter::url() const { 
    return std::string(req_.url); 
}

const std::string& CrowRequestAdapter::method() const { 
    return method_str_; 
}

// Return by value to avoid reference to temporary
std::string CrowRequestAdapter::body() const { 
    return std::string(req_.body); 
}

CrowResponseAdapter::CrowResponseAdapter(::crow::response &res) : res_(res) {}

void CrowResponseAdapter::setCode(int code) { 
    res_.code = code; 
}

int CrowResponseAdapter::getCode() const { 
    return res_.code; 
}

void CrowResponseAdapter::setBody(const std::string &body) { 
    res_.body = body; 
}

void CrowResponseAdapter::end() { 
    res_.end(); 
}

// Return by value to avoid reference to temporary
std::string CrowResponseAdapter::getBody() const { 
    return std::string(res_.body); 
}

std::unique_ptr<HttpResponse> makeResponse(::crow::response &res) {
    return std::make_unique<CrowResponseAdapter>(res);
}

std::unique_ptr<HttpRequest> makeRequest(::crow::request &req) {
    return std::make_unique<CrowRequestAdapter>(req);
}

#define API_PREFIX "/api/v1"

/**
 * @brief Setup the SEP API routes in a Crow application
 */
void setupSepApiRoutes(::crow::Crow<>* app) {
    if (!app) return;
    
    using json_t = nlohmann::json;
    auto& engine = SepEngine::getInstance();
    
    // Helper lambda to create JSON responses
    auto makeJsonResponse = [](const json_t& data, int status = 200) {
        ::crow::response res(status);
        res.set_header("Content-Type", "application/json");
        res.body = data.dump();
        return res;
    };
    
    // Helper lambda to handle API errors
    auto handleApiError = [makeJsonResponse](const std::exception& e) {
        json_t error_response;
        error_response["error"] = e.what();
        error_response["status"] = "error";
        return makeJsonResponse(error_response, 500);
    };
    
    // Route: Health check
    app->route_dynamic(API_PREFIX "/health")
        .methods(::crow::HTTPMethod::Get)
        ([&engine, makeJsonResponse]() {
            try {
                auto health = engine.getHealthStatus();
                return makeJsonResponse(health);
            } catch (const std::exception& e) {
                json_t error;
                error["error"] = e.what();
                return makeJsonResponse(error, 500);
            }
        });
    
    // Route: Process patterns
    app->route_dynamic(API_PREFIX "/pattern/evolve")
        .methods(::crow::HTTPMethod::Post)
        ([&engine, makeJsonResponse, handleApiError](const ::crow::request& req) {
            try {
                // Use our helper to parse JSON from crow request body
                auto body = parse_json(std::string(req.body));
                auto result = engine.processPatterns(body);
                return makeJsonResponse(result);
            } catch (const std::exception& e) {
                return handleApiError(e);
            }
        });
    
    // Route: Extract embeddings
    app->route_dynamic(API_PREFIX "/embeddings/extract")
        .methods(::crow::HTTPMethod::Post)
        ([&engine, makeJsonResponse, handleApiError](const ::crow::request& req) {
            try {
                auto body = parse_json(std::string(req.body));
                auto result = engine.extractEmbeddings(body);
                return makeJsonResponse(result);
            } catch (const std::exception& e) {
                return handleApiError(e);
            }
        });
    
    // Route: Query memory
    app->route_dynamic(API_PREFIX "/memory/query")
        .methods(::crow::HTTPMethod::Post)
        ([&engine, makeJsonResponse, handleApiError](const ::crow::request& req) {
            try {
                auto body = parse_json(std::string(req.body));
                auto result = engine.queryMemory(body);
                return makeJsonResponse(result);
            } catch (const std::exception& e) {
                return handleApiError(e);
            }
        });
    
    // Route: Update memory
    app->route_dynamic(API_PREFIX "/memory/update")
        .methods(::crow::HTTPMethod::Post)
        ([&engine, makeJsonResponse, handleApiError](const ::crow::request& req) {
            try {
                auto body = parse_json(std::string(req.body));
                auto result = engine.updateMemory(body);
                return makeJsonResponse(result);
            } catch (const std::exception& e) {
                return handleApiError(e);
            }
        });
    
    // Route: Validate contexts
    app->route_dynamic(API_PREFIX "/context/validate")
        .methods(::crow::HTTPMethod::Post)
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
        .methods(::crow::HTTPMethod::Post)
        ([&engine, makeJsonResponse, handleApiError](const ::crow::request& req) {
            try {
                auto body = parse_json(std::string(req.body));
                auto result = engine.blendContexts(body);
                return makeJsonResponse(result);
            } catch (const std::exception& e) {
                return handleApiError(e);
            }
        });
    
    // Route: Analyze pattern stability
    app->route_dynamic(API_PREFIX "/pattern/analyze")
        .methods(::crow::HTTPMethod::Post)
        ([&engine, makeJsonResponse, handleApiError](const ::crow::request& req) {
            try {
                auto body = parse_json(std::string(req.body));
                auto result = engine.calculateSimilarity(body);
                return makeJsonResponse(result);
            } catch (const std::exception& e) {
                return handleApiError(e);
            }
        });
    
    // Route: Get pattern history
    app->route_dynamic(API_PREFIX "/patterns/history")
        .methods(::crow::HTTPMethod::Post)
        ([&engine, makeJsonResponse, handleApiError](const ::crow::request& req) {
            try {
                auto body = parse_json(std::string(req.body));
                auto result = engine.getPatternHistory(body);
                return makeJsonResponse(result);
            } catch (const std::exception& e) {
                return handleApiError(e);
            }
        });
    
    // Route: Quantum state management
    app->route_dynamic(API_PREFIX "/quantum/state")
        .methods(::crow::HTTPMethod::Get)
        ([&engine, makeJsonResponse, handleApiError]() {
            try {
                auto result = engine.getQuantumState();
                return makeJsonResponse(result);
            } catch (const std::exception& e) {
                return handleApiError(e);
            }
        });
}

} // namespace sep::api

---

// include/api/crow_adapter.h
/**
 * @file crow_adapter.h
 * @brief Header file for the Crow adapter for the SEP Engine API
 */

#pragma once

// Define CROW_DISABLE_RTTI first since we're using with CUDA
#define CROW_DISABLE_RTTI 1

// Forward declarations - use class instead of struct to match crow's definitions
namespace crow {
    template <typename... Middlewares>
    class Crow;
    class request;
    class response;
}

#include <memory>
#include <string>
#include "api/types.h"

namespace sep::api {

/**
 * @brief Setup the SEP API routes in a Crow application
 *
 * This function sets up the routes for the SEP Engine API in a Crow application.
 *
 * @param app The Crow application instance
 */
void setupSepApiRoutes(::crow::Crow<>* app);

// Adapter for crow::request to HttpRequest
class CrowRequestAdapter : public HttpRequest {
public:
  explicit CrowRequestAdapter(::crow::request &req);
  
  // Changed to return by value to avoid reference to temporary
  std::string url() const override;
  const std::string &method() const override;  // This one is safe - method_str_ is a member
  std::string body() const override;

 private:
  ::crow::request &req_;
  std::string method_str_;
};

// Adapter for crow::response to HttpResponse
class CrowResponseAdapter : public HttpResponse {
public:
  explicit CrowResponseAdapter(::crow::response &res);
  void setCode(int code) override;
  int getCode() const override;
  void setBody(const std::string &body) override;
  void end() override;
  
  // Changed to return by value to avoid reference to temporary
  std::string getBody() const override;

 private:
  ::crow::response &res_;
};

std::unique_ptr<HttpResponse> makeResponse(::crow::response &res);
std::unique_ptr<HttpRequest> makeRequest(::crow::request &req);

}  // namespace sep::api


---

// src/api/server.cpp - Key fixes to apply

// At the top of the file, ensure these includes:
#include "api/server.h"
#include "api/ollama_client.h"  // Include full definition, not just forward declaration
#include "api/json_helpers.h"    // Our JSON compatibility helpers
#include "crow/crow.h"           // Full crow definition
#include <spdlog/spdlog.h>

// ... existing code ...

// In the constructor, ensure app_ is properly initialized:
SEPApiServer::SEPApiServer(const ::sep::config::APIConfig& config)
    : config_(config), running_(false) {
    // Initialize the Crow app with middlewares
    app_ = std::make_unique<::crow::Crow<RateLimitMiddleware, AuthMiddleware>>();
    
    // Initialize logger
    setup_logging();
    
    // Initialize clients if needed
    if (config_.ollama.enabled) {
        ollama_client_ = std::make_unique<OllamaClient>(config_.ollama);
    }
}

// Fix the app configuration (around line 81):
bool SEPApiServer::start() {
    if (!app_) return false;
    
    // Split the chaining to avoid issues
    app_->port(config_.port);
    app_->multithreaded();
    
    // Setup middleware and routes
    setup_middleware();
    setup_routes();
    
    return true;
}

// In setup_routes(), fix all method literals:
void SEPApiServer::setup_routes() {
    if (!app_) return;

    auto& engine = SepEngine::getInstance();

    // Health check endpoint - fix GET method
    app_->route_dynamic("/api/v1/health")
        .methods(::crow::HTTPMethod::Get)  // Changed from "GET"_method
        ([this, &engine](const ::crow::request& req) {
            // ... existing implementation ...
        });

    // Process patterns endpoint - fix POST method
    app_->route_dynamic("/api/v1/pattern/evolve")
        .methods(::crow::HTTPMethod::Post)  // Changed from "POST"_method
        ([this, &engine](const ::crow::request& req) {
            auto start_time = std::chrono::steady_clock::now();
            
            try {
                // Use our JSON helper for parsing
                nlohmann::json request_data = parse_json(std::string(req.body));
                
                // Process patterns through SEP engine
                auto result = engine.processPatterns(request_data);
                auto response_data = applyCoherenceModulation(result);
                
                auto end_time = std::chrono::steady_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
                    end_time - start_time).count();
                
                logRequest(req, HTTP_OK, response_data.dump(), duration);
                return makeCrowJsonResponse(HTTP_OK, response_data);
                
            } catch (const std::exception& e) {
                auto end_time = std::chrono::steady_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
                    end_time - start_time).count();
                
                auto error_crow = handleCrowError(
                    "Pattern evolution failed: " + std::string(e.what()), 
                    HTTP_INTERNAL_ERROR);
                logRequest(req, HTTP_INTERNAL_ERROR, error_crow.dump(), duration);
                return makeCrowJsonResponse(HTTP_INTERNAL_ERROR, error_crow);
            }
        });

    // Memory query endpoint
    app_->route_dynamic("/api/v1/memory/query")
        .methods(::crow::HTTPMethod::Post)  // Changed from "POST"_method
        ([this, &engine](const ::crow::request& req) {
            // ... implementation with parse_json helper ...
        });

    // Pattern history endpoint
    app_->route_dynamic("/api/v1/patterns/history")
        .methods(::crow::HTTPMethod::Post)  // Changed from "POST"_method
        ([this, &engine](const ::crow::request& req) {
            // ... implementation ...
        });

    // Embeddings extraction endpoint
    app_->route_dynamic("/api/v1/embeddings/extract")
        .methods(::crow::HTTPMethod::Post)  // Changed from "POST"_method
        ([this, &engine](const ::crow::request& req) {
            // ... implementation ...
        });

    // Pattern analysis endpoint
    app_->route_dynamic("/api/v1/pattern/analyze")
        .methods(::crow::HTTPMethod::Post)  // Changed from "POST"_method
        ([this, &engine](const ::crow::request& req) {
            // ... implementation ...
        });

    // Quantum state endpoint
    app_->route_dynamic("/api/v1/quantum/state")
        .methods(::crow::HTTPMethod::Get)  // Changed from "GET"_method
        ([this, &engine](const ::crow::request& req) {
            // ... implementation ...
        });
}

// Fix the logging method to handle shim::string properly (around line 270):
void SEPApiServer::logRequest(const ::crow::request& req, int status_code, 
                              const std::string& response_body, int64_t duration_ms) {
    if (!logger_) return;
    
    // Convert all parameters to std::string for logging
    std::string method_name = std::string(::crow::method_name(req.method));
    std::string url = std::string(req.url);
    
    logger_->info("Request: {} {} - Status: {} - Duration: {}ms", 
                  method_name, url, status_code, duration_ms);
    
    if (logger_->level() <= spdlog::level::debug) {
        logger_->debug("Response body: {}", response_body);
    }
}

// Helper to convert HttpRequest to crow request for logging
void SEPApiServer::logRequest(const HttpRequest& req, int code, 
                              const std::string& body, int64_t duration) {
    if (!logger_) return;
    
    // Use the virtual methods which return std::string
    logger_->info("Request: {} {} - Status: {} - Duration: {}ms",
                  req.method(), req.url(), code, duration);
                  
    if (logger_->level() <= spdlog::level::debug) {
        logger_->debug("Response body: {}", body);
    }
}  // namespace sep::api


---

// include/api/types.h - Updated HTTP interface definitions
#pragma once

#include <string>
#include <map>
#include <vector>

namespace sep::api {

// HTTP status codes
constexpr int HTTP_OK = 200;
constexpr int HTTP_CREATED = 201;
constexpr int HTTP_BAD_REQUEST = 400;
constexpr int HTTP_UNAUTHORIZED = 401;
constexpr int HTTP_NOT_FOUND = 404;
constexpr int HTTP_INTERNAL_ERROR = 500;
constexpr int HTTP_SERVICE_UNAVAILABLE = 503;

/**
 * @brief Abstract interface for HTTP requests
 * 
 * Note: Methods return by value to avoid reference-to-temporary issues
 * when adapting from different HTTP libraries
 */
class HttpRequest {
public:
    virtual ~HttpRequest() = default;
    
    // Return by value for safety with different string types
    virtual std::string url() const = 0;
    virtual std::string method() const = 0;
    virtual std::string body() const = 0;
    
    // Optional: Add header access if needed
    virtual std::string getHeader(const std::string& name) const {
        return "";
    }
};

/**
 * @brief Abstract interface for HTTP responses
 */
class HttpResponse {
public:
    virtual ~HttpResponse() = default;
    
    virtual void setCode(int code) = 0;
    virtual int getCode() const = 0;
    virtual void setBody(const std::string& body) = 0;
    virtual std::string getBody() const = 0;
    virtual void end() = 0;
    
    // Optional: Add header setting if needed
    virtual void setHeader(const std::string& name, const std::string& value) {
        // Default no-op implementation
    }
};

// Health metrics structure
struct HealthMetrics {
    bool healthy = true;
    std::string status = "ok";
    int64_t uptime_ms = 0;
    std::map<std::string, double> metrics;
};

// API configuration helpers
struct RateLimitConfig {
    int requests_per_minute = 60;
    bool enabled = true;
};

struct AuthConfig {
    bool enabled = false;
    std::vector<std::string> tokens;
};

} // namespace sep::api