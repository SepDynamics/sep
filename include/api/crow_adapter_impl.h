/**
 * @file crow_adapter_impl.h
 * @brief Implementation of the Crow adapter classes for the SEP Engine API
 *
 * This file provides the implementation of the adapter classes needed to
 * integrate the SEP Engine API with the Crow web framework.
 */

#pragma once

// Define CROW_DISABLE_RTTI first since we're using with CUDA
#define CROW_DISABLE_RTTI 1

// Include our fixed headers first
#include "crow/common.h"
#include "crow/http_request.h"
#include "crow/http_response.h"
#include "crow/crow_isolation.h"

#include "api/crow_adapter.h"
#include <memory>
#include <string>

namespace sep::api {

// Implementation of CrowRequestAdapter
CrowRequestAdapter::CrowRequestAdapter(::crow::request &req) : req_(req) {
    method_str_ = ::crow::method_name(req.method);
}

const std::string &CrowRequestAdapter::url() const { 
    return req_.url; 
}

const std::string &CrowRequestAdapter::method() const { 
    return method_str_; 
}

const std::string &CrowRequestAdapter::body() const { 
    return req_.body; 
}

// Implementation of CrowResponseAdapter
CrowResponseAdapter::CrowResponseAdapter(::crow::response &res) : res_(res) {}

void CrowResponseAdapter::setCode(int code) { 
    res_.code = code; 
}

int CrowResponseAdapter::getCode() const { 
    return res_.code; 
}

void CrowResponseAdapter::setBody(const std::string &body) { 
    res_.body = body; 
}

void CrowResponseAdapter::end() { 
    res_.end(); 
}

const std::string &CrowResponseAdapter::getBody() const { 
    return res_.body; 
}

// Factory functions
std::unique_ptr<HttpResponse> makeResponse(::crow::response &res) {
    return std::make_unique<CrowResponseAdapter>(res);
}

std::unique_ptr<HttpRequest> makeRequest(::crow::request &req) {
    return std::make_unique<CrowRequestAdapter>(req);
}

} // namespace sep::api