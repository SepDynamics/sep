#pragma once

#include <atomic>
#include <chrono>
#include <map>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

namespace sep::api {

constexpr int HTTP_OK = 200;
constexpr int HTTP_CREATED = 201;
constexpr int HTTP_BAD_REQUEST = 400;
constexpr int HTTP_UNAUTHORIZED = 401;
constexpr int HTTP_NOT_FOUND = 404;
constexpr int HTTP_INTERNAL_ERROR = 500;
constexpr int HTTP_SERVICE_UNAVAILABLE = 503;

class HttpRequest {
public:
    virtual ~HttpRequest() = default;

    virtual std::string url() const = 0;
    virtual std::string method() const = 0;
    virtual std::string body() const = 0;

    virtual std::string getHeader(const std::string& name) const { return ""; }
};

class HttpResponse {
public:
    virtual ~HttpResponse() = default;

    virtual void setCode(int code) = 0;
    virtual int getCode() const = 0;
    virtual void setBody(const std::string& body) = 0;
    virtual std::string getBody() const = 0;
    virtual void end() = 0;

    virtual void setHeader(const std::string& name, const std::string& value) {
        (void)name;
        (void)value;
    }
};

// Basic health metrics used by the HTTP client
struct HealthMetrics {
    std::atomic<size_t> totalRequests{0};
    std::atomic<size_t> successfulRequests{0};
    std::atomic<size_t> failedRequests{0};
    std::atomic<size_t> timeoutRequests{0};
    std::atomic<size_t> rateLimitedCount{0};
    std::atomic<double> averageResponseTime{0.0};
    std::chrono::steady_clock::time_point lastRequestTime;
    std::chrono::steady_clock::time_point startTime;
    std::chrono::milliseconds lastResponseTime{0};
    std::chrono::system_clock::time_point lastSuccessTime;
    std::chrono::system_clock::time_point lastErrorTime;
    int lastErrorCode{0};
};

struct RateLimitConfig {
    int requests_per_minute = 60;
    bool enabled = true;
};

struct AuthConfig {
    bool enabled = false;
    std::vector<std::string> tokens;
};

// Error codes returned by API operations
enum class ErrorCode {
    Success = 0,
    InvalidArgument,
    InvalidParameter,
    InvalidOperation,
    ResourceNotFound,
    OutOfMemory,
    InvalidState,
    SystemError,
    CudaError,
    ProcessingError,
    ApiError,
    GeneralError,
    BufferTooSmall,
    Unknown
};

// Request priority for rate limiting
enum class Priority { LOW = 0, NORMAL = 1, HIGH = 2, CRITICAL = 3 };

struct APIRequest {
    std::string method;
    std::string url;
    std::string body;
    std::map<std::string, std::string> headers;
    Priority priority = Priority::NORMAL;
    std::chrono::milliseconds timeout{5000};
    std::string requestId;
};

struct APIResponse {
    int statusCode = 0;
    std::string body;
    std::map<std::string, std::string> headers;
    std::chrono::milliseconds responseTime{0};
    std::string requestId;
    bool success = false;
    struct Error {
        ErrorCode code{ErrorCode::Success};
        std::string message;
    } error;
};

// Utility helpers for API responses
nlohmann::json make_error_response(ErrorCode code, const std::string& message);
bool validate_fields(const nlohmann::json& data,
                     const std::vector<std::string>& fields,
                     nlohmann::json& error);

} // namespace sep::api
