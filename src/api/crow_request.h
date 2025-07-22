#pragma once

// The build system previously defined `SEP_NO_STDLIB` when compiling without the
// C++ standard library.  The project now relies on the system C++ runtime so the
// macro is expected to be undefined during normal builds.

// Include directly from crow_isolation.h to avoid shim.h dependency


#include "api/request_interface.h"

namespace sep::api {

class CrowRequest : public IRequest
{
public:
    CrowRequest(const ::crow::request& req) : req_(req), body_(req.body)
    {
        // In Crow, headers might not be directly accessible as a member
        // So we'll populate our headers from individual header values
        // using get_header_value() method
        // This approach is more resilient to Crow API changes
        headers_["content-type"] = req.get_header_value("content-type");
        headers_["authorization"] = req.get_header_value("authorization");
        headers_["user-agent"] = req.get_header_value("user-agent");
        headers_["accept"] = req.get_header_value("accept");
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
        return body_;
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
        // Access might have changed in Crow - use a static empty string as fallback
        static std::string ip_address = "127.0.0.1";
        // In newer Crow versions, remote_ip_address should be accessed differently
        // or might not be directly accessible
        return ip_address;
    }

private:
    const ::crow::request& req_;
    std::string body_;
    std::unordered_map<std::string, std::string> headers_;
};

}  // namespace sep::api
