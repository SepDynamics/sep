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

    virtual std::string                                         method() const  = 0;
    virtual std::string                                         url() const     = 0;
    virtual const std::string&                                  body() const    = 0;
    virtual const std::unordered_map<std::string, std::string>& headers() const = 0;
    // Added virtual declarations for previously missing methods
    virtual std::string        get_header_value(const std::string& key) const = 0;
    virtual const std::string& get_remote_ip() const                          = 0;
};

}  // namespace sep::api
