#pragma once

#include <string>
#include <unordered_map>
#include "crow/app.h"
#include "api/types.h"

namespace sep {
namespace api {

/**
 * @brief Adapts Crow's request object to provide a consistent interface for request handling
 */
class CrowRequestAdapter : public HttpRequest {
public:
    /**
     * @brief Constructs a new adapter around a Crow request
     * @param req Reference to the Crow request to adapt
     */
    explicit CrowRequestAdapter(crow::request& req);

    /**
     * @brief Gets the request URL
     * @return The request URL as a copy
     */
    std::string url() const override;

    /**
     * @brief Gets the HTTP method
     * @return The request method as a copy
     */
    std::string method() const override;

    /**
     * @brief Gets the request body
     * @return The request body as a copy
     */
    std::string body() const override;

    /**
     * @brief Gets the value of a specific header
     * @param name The header name to look up
     * @return The header value if found, empty string otherwise
     */
    std::string getHeader(const std::string& name) const override;

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