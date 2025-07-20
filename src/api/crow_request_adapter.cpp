#include "api/crow_request_adapter.h"

#include <string>

namespace sep {
namespace api {

CrowRequestAdapter::CrowRequestAdapter(crow::request& req) : request_(req) {}

shim::string CrowRequestAdapter::getHeader(const shim::string& name) const
{
    return request_.get_header_value(name);
}

shim::string CrowRequestAdapter::body() const { return shim::string(request_.body); }

shim::string CrowRequestAdapter::method() const
{
    return shim::string(crow::method_name(request_.method));
}

shim::string CrowRequestAdapter::url() const { return shim::string(request_.url); }

shim::string CrowRequestAdapter::getQueryParam(const shim::string& name) const
{
    // Parse query parameters from URL
    shim::string url = request_.url;
    size_t pos = url.find('?');
    if (pos == shim::string::npos)
    {
        return shim::string();
    }

    shim::string query = url.substr(pos + 1);
    shim::string param = name + "=";
    pos = query.find(param);
    if (pos == shim::string::npos)
    {
        return shim::string();
    }

    size_t start = pos + param.length();
    size_t end = query.find('&', start);
    if (end == shim::string::npos)
    {
        end = query.length();
    }

    return query.substr(start, end - start);
}

} // namespace api
} // namespace sep