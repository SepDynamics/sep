#pragma once

// This is a fixed version of the http_request.h file from the Crow framework
// It provides stub implementations for HTTP request functionality

// Include the isolation header which pulls in the real Crow request
#include "crow_isolation.h"

namespace crow {
    // The request class is already defined in crow_isolation.h
    // This file just ensures that the HTTPMethod::GET is used correctly
    
    // Create a factory function to create requests with the correct method
    inline request make_request(HTTPMethod method = HTTPMethod::GET) {
        request req;
        req.method = method;
        return req;
    }
    
    // Add any additional HTTP request functionality needed here
    
    // Helper function to convert string to HTTPMethod
    inline HTTPMethod method_from_string(const char* str) {
        if (strcmp(str, "GET") == 0) return HTTPMethod::GET;
        if (strcmp(str, "POST") == 0) return HTTPMethod::POST;
        if (strcmp(str, "PUT") == 0) return HTTPMethod::PUT;
        if (strcmp(str, "DELETE") == 0) return HTTPMethod::DELETE;
        if (strcmp(str, "HEAD") == 0) return HTTPMethod::HEAD;
        if (strcmp(str, "OPTIONS") == 0) return HTTPMethod::OPTIONS;
        if (strcmp(str, "PATCH") == 0) return HTTPMethod::PATCH;
        if (strcmp(str, "CONNECT") == 0) return HTTPMethod::CONNECT;
        if (strcmp(str, "TRACE") == 0) return HTTPMethod::TRACE;
        return HTTPMethod::GET;  // Default to GET
    }
    
    // String literal operator for HTTPMethod
    inline HTTPMethod operator""_method(const char* str, size_t) {
        return method_from_string(str);
    }
}