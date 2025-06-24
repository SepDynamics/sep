#pragma once

#include <string>
#include <map>
#include <vector>

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

struct HealthMetrics {
    bool healthy = true;
    std::string status = "ok";
    int64_t uptime_ms = 0;
    std::map<std::string, double> metrics;
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
