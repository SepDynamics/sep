#include "api/crow_request_adapter.h"

namespace sep {
namespace api {

CrowRequestAdapter::CrowRequestAdapter(crow::request& req) : request_(req) {}

const std::string& CrowRequestAdapter::getHeader(const std::string& key) const {
    static const std::string empty;
    auto it = request_.headers.find(key);
    return it != request_.headers.end() ? it->second : empty;
}

const std::string& CrowRequestAdapter::getBody() const {
    return request_.body;
}

const std::string& CrowRequestAdapter::getMethod() const {
    return request_.method;
}

const std::string& CrowRequestAdapter::getUrl() const {
    return request_.url;
}

const std::unordered_map<std::string, std::string>& CrowRequestAdapter::getHeaders() const {
    return request_.headers;
}

const std::unordered_map<std::string, std::string>& CrowRequestAdapter::getParams() const {
    return request_.url_params;
}

} // namespace api
} // namespace sep