#pragma once

// The build system previously defined `SEP_NO_STDLIB` when compiling without the
// C++ standard library.  The project now relies on the system C++ runtime so the
// macro is expected to be undefined during normal builds.

#include <crow/http_request.h>

#include "api/request_interface.h"

namespace sep::api {

class CrowRequest : public IRequest
{
public:
    CrowRequest(const ::crow::request& req) : req_(req)
    {
        for (const auto& h : req_.headers)
        {
            headers_[h.first] = h.second;
        }
    }

    std::string method() const override
    {
        return ::crow::method_name(req_.method);
    }
    
    std::string url() const override
    {
        return req_.url;
    }
    
    const std::string& body() const override
    {
        return req_.body;
    }

    const std::unordered_map<std::string, std::string>& headers() const override
    {
        return headers_;
    }

    std::string get_header_value(const std::string& key) const override
    {
        // Call the stub's get_header_value
        return req_.get_header_value(key);
    }

    const std::string& get_remote_ip() const override
    {
        return req_.remote_ip_address;
    }

private:
    const ::crow::request& req_;
    std::unordered_map<std::string, std::string> headers_;
};

}  // namespace sep::api
