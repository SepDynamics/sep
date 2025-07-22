#pragma once

#include <string>
#include <atomic>
#include <chrono>
#include <map>
#include <vector>
#include <nlohmann/json.hpp>

namespace sep::api {

constexpr int HTTP_OK = 200;
constexpr int HTTP_CREATED = 201;
constexpr int HTTP_ACCEPTED = 202;
constexpr int HTTP_NO_CONTENT = 204;
constexpr int HTTP_BAD_REQUEST = 400;
constexpr int HTTP_UNAUTHORIZED = 401;
constexpr int HTTP_FORBIDDEN = 403;
constexpr int HTTP_NOT_FOUND = 404;
constexpr int HTTP_METHOD_NOT_ALLOWED = 405;
constexpr int HTTP_CONFLICT = 409;
constexpr int HTTP_INTERNAL_ERROR = 500;
constexpr int HTTP_NOT_IMPLEMENTED = 501;
constexpr int HTTP_SERVICE_UNAVAILABLE = 503;
constexpr int HTTP_TOO_MANY_REQUESTS = 429;

enum class Status { OK = 0, ERROR = 1 };
enum class Priority { LOW = 0, NORMAL = 1, HIGH = 2, CRITICAL = 3 };

enum HttpMethod {
    HTTP_GET,
    HTTP_POST,
    HTTP_PUT,
    HTTP_DELETE,
    HTTP_PATCH,
    HTTP_OPTIONS,
    HTTP_HEAD
};

class HttpRequest {
public:
    virtual ~HttpRequest() = default;

    virtual std::string url() const = 0;
    virtual std::string method() const = 0;
    virtual std::string body() const = 0;

    virtual std::string getHeader(const std::string& name) const {
        return "";
    }
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
        (void)name; (void)value;
    }
};

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

} // namespace sep::api

namespace sep::ollama {

struct OllamaConfig {
    std::string host{"http://127.0.0.1:11434"};
    std::string model{"llama2"};
};

struct GenerateRequest {
    std::string model;
    std::string prompt;
    std::string system;
    bool stream{false};
};

struct GenerateResponse {
    std::string response;
    bool done{false};
    std::string model;
};

struct EmbeddingRequest {
    std::string model;
    std::string prompt;
};

struct EmbeddingResponse {
    std::vector<float> embedding;
};

}  // namespace sep::ollama
