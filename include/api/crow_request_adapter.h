#pragma once

#include <string>
#include <unordered_map>
#include "crow/app.h"

namespace sep {
namespace api {

/**
 * @brief Adapts Crow's request object to provide a consistent interface for request handling
 */
class CrowRequestAdapter {
public:
    /**
     * @brief Constructs a new adapter around a Crow request
     * @param req Reference to the Crow request to adapt
     */
    explicit CrowRequestAdapter(crow::request& req);

    /**
     * @brief Gets the value of a specific header
     * @param key The header key to look up
     * @return The header value if found, empty string otherwise
     */
    std::string getHeader(const std::string& key) const;

    /**
     * @brief Gets the request body content
     * @return The request body as a copy
     */
    std::string getBody() const;

    /**
     * @brief Gets the HTTP method of the request
     * @return The request method as a string copy
     */
    std::string getMethod() const;

    /**
     * @brief Gets the request URL
     * @return The request URL as a copy
     */
    std::string getUrl() const;

    /**
     * @brief Gets a URL parameter value
     * @param name The parameter name
     * @return The parameter value if found, empty string otherwise
     */
    std::string getQueryParam(const std::string& name) const;

private:
    crow::request& request_; ///< Reference to the underlying Crow request
};

} // namespace api
} // namespace sep