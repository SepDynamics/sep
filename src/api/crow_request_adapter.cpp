#include "api/crow_request_adapter.h"
#include <string> // For std::string

namespace sep {
namespace api {

CrowRequestAdapter::CrowRequestAdapter(crow::request& req) : request_(req) {}

std::string CrowRequestAdapter::getHeader(const std::string& name) const {
    return request_.get_header_value(name);
}

std::string CrowRequestAdapter::body() const {
    return std::string(request_.body);
}

std::string CrowRequestAdapter::method() const {
    return std::string(crow::method_name(request_.method));
}

std::string CrowRequestAdapter::url() const {
    return std::string(request_.url);
}

std::string CrowRequestAdapter::getQueryParam(const std::string& name) const {
    // Parse query parameters from URL
    std::string url = request_.url;
    size_t pos = url.find('?');
    if (pos == std::string::npos) {
        return std::string();
    }

    std::string query = url.substr(pos + 1);
    std::string param = name + "=";
    pos = query.find(param);
    if (pos == std::string::npos) {
        return std::string();
    }

    size_t start = pos + param.length();
    size_t end = query.find('&', start);
    if (end == std::string::npos) {
        end = query.length();
    }

    return query.substr(start, end - start);
}

} // namespace api
} // namespace sep