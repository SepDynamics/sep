#pragma once

#include <string>
#include <unordered_map>
#include <vector>

namespace sep::api {

// Abstract base class for HTTP requests
class IRequest
{
public:
    virtual ~IRequest() = default;

    virtual shim::string method() const = 0;
    virtual shim::string url() const = 0;
    virtual const shim::string& body() const = 0;
    virtual const std::unordered_map<shim::string, shim::string>& headers() const = 0;
    // Added virtual declarations for previously missing methods
    virtual shim::string get_header_value(const shim::string& key) const = 0;
    virtual const shim::string& get_remote_ip() const = 0;
};

}  // namespace sep::api
