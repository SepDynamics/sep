#pragma once

#include <string>

namespace sep {
namespace core {

/**
 * @brief Abstract interface for HTTP requests
 */
class HttpRequest {
public:
    virtual ~HttpRequest() = default;

    /**
     * @brief Gets the request URL
     * @return The request URL
     */
    virtual std::string url() const = 0;

    /**
     * @brief Gets the HTTP method
     * @return The request method
     */
    virtual std::string method() const = 0;

    /**
     * @brief Gets the request body
     * @return The request body
     */
    virtual std::string body() const = 0;

    /**
     * @brief Gets the value of a specific header
     * @param name The header name to look up
     * @return The header value if found, empty string otherwise
     */
    virtual std::string getHeader(const std::string& name) const = 0;
};

} // namespace core
} // namespace sep