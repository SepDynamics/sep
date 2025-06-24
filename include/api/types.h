#ifndef API_TYPES_H
#define API_TYPES_H


/**
 * @file types.h
 * @brief API types and constants for the SEP Engine API
 * 
 * This file defines HTTP status codes and other types used in the SEP Engine API.
 */

#pragma once

#include <string>
#include <atomic>
#include <chrono>
#include <map>
#include <vector>
#include <nlohmann/json.hpp>

namespace sep::api {

/**
 * @brief HTTP status codes
 * 
 * These are the standard HTTP status codes used in the API responses.
 */
// HTTP status codes
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

// HTTP methods
enum HttpMethod {
    HTTP_GET,
    HTTP_POST,
    HTTP_PUT,
    HTTP_DELETE,
    HTTP_PATCH,
    HTTP_OPTIONS,
    HTTP_HEAD
};

// Abstract base class for HTTP requests
class HttpRequest {
public:
    virtual ~HttpRequest() = default;

    // Return by value for safety with different string types
    virtual std::string url() const = 0;
    virtual std::string method() const = 0;
    virtual std::string body() const = 0;

    // Optional: Add header access if needed
    virtual std::string getHeader(const std::string& name) const { return ""; }
};

// Abstract base class for HTTP responses
class HttpResponse {
public:
    virtual ~HttpResponse() = default;

    virtual void setCode(int code) = 0;
    virtual int getCode() const = 0;
    virtual void setBody(const std::string& body) = 0;
    virtual std::string getBody() const = 0;
    virtual void end() = 0;

    // Optional: Add header setting if needed
    virtual void setHeader(const std::string& name, const std::string& value) {}
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

// Utility helpers for API responses
nlohmann::json make_error_response(ErrorCode code, const std::string& message);
bool validate_fields(const nlohmann::json& data,
                     const std::vector<std::string>& fields,
                     nlohmann::json& error);

}  // namespace sep::api
 
#endif  // API_TYPES_H

namespace sep::ollama {

// Configuration for Ollama client
struct OllamaConfig {
  std::string host{"http://127.0.0.1:11434"};
  std::string model{"llama2"};
};

// Request structure for text generation
struct GenerateRequest {
  std::string model;
  std::string prompt;
  std::string system;
  bool stream{false};
};

// Response structure for text generation
struct GenerateResponse {
  std::string response;
  bool done{false};
  std::string model;
};

// Request structure for embeddings
struct EmbeddingRequest {
  std::string model;
  std::string prompt;
};

// Response structure for embeddings
struct EmbeddingResponse {
  std::vector<float> embedding;
};

}  // namespace sep::ollama