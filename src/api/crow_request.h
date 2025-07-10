#pragma once

// The build system previously defined `SEP_NO_STDLIB` when compiling without the
// C++ standard library.  The project now relies on the system C++ runtime so the
// macro is expected to be undefined during normal builds.

// Include directly from crow_isolation.h to avoid shim.h dependency
#include "crow/crow_isolation.h"

#include "api/request_interface.h"

namespace sep::api {

class CrowRequest : public IRequest
{
public:
    CrowRequest(const crow::request& req) : req_(req), body_(req.body.c_str())
    {
        // In Crow isolation shim, we populate headers with empty values
        // In the real implementation, these would be populated from the actual request
        headers_["content-type"] = "";
        headers_["authorization"] = "";
        headers_["user-agent"] = "";
        headers_["accept"] = "";
    }

    std::string method() const override { return crow::method_name(req_.method); }

    std::string url() const override
    {
        // Convert crow_string to std::string by using c_str()
        return std::string(req_.url.c_str());
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
        // Use the cached headers instead of calling the stub directly
        // This avoids the conversion from std::string to crow_string
        auto it = headers_.find(key);
        if (it != headers_.end()) {
            return it->second;
        }
        return "";
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
    const crow::request& req_;
    std::string body_;
    std::unordered_map<std::string, std::string> headers_;
};

}  // namespace sep::api
