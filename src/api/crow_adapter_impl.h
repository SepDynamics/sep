/**
 * @file crow_adapter_impl.h
 * @brief Implementation of adapters for integrating with the Crow web framework
 * 
 * This file provides the necessary adapter implementations to bridge the SEP Engine
 * API with the Crow web framework.
 */

#pragma once

// Include Crow headers with correct path
#include "../extern/crow/crow_isolation.h"

#include <string>
#include <memory>

namespace sep {
namespace api {

/**
 * Adapter for Crow requests
 * Provides a consistent interface to access Crow request data
 */
class CrowRequestAdapter {
public:
    explicit CrowRequestAdapter(const ::crow::request& req) : req_(req) {
        // Get method as string for consistent interface
        method_str_ = ::crow::method_name(req.method);
    }

    std::string url() const { return req_.url; }
    std::string method() const { return method_str_; }
    std::string body() const { return req_.body; }

    std::string get_header_value(const std::string& key) const {
        return req_.get_header_value(key);
    }

    // Create a JSON object from the request body
    std::string parse_json() const {
        try {
            // Just return the raw body as string for parsing by caller
            return std::string(req_.body);
        } catch (...) {
            return std::string(); // Return empty string on error
        }
    }

private:
    const ::crow::request& req_;
    std::string method_str_;
};

/**
 * Adapter for Crow responses
 * Provides a consistent interface to set Crow response data
 */
class CrowResponseAdapter {
public:
    explicit CrowResponseAdapter(::crow::response& res) : res_(res) {}

    void set_code(int code) { res_.code = code; }
    int get_code() const { return res_.code; }
    void set_body(const std::string& body) { res_.body = body; }
    std::string get_body() const { return res_.body; }
    void set_header(const std::string& key, const std::string& value) {
        res_.set_header(key, value);
    }

private:
    ::crow::response& res_;
};

} // namespace api
} // namespace sep