#pragma once

// This is a fixed version of the http_response.h file from the Crow framework
// It provides stub implementations for HTTP response functionality

// Include our own headers
#include "compat/shim.h"
#include "common.h"
#include "crow_isolation.h"

// The response class is already defined in crow_isolation.h
// This file just ensures that it's properly included

namespace crow {
    // Create a factory function to create responses with the correct status code
    inline response make_response(int status_code = 200) {
        return response(status_code);
    }
    
    // Add any additional HTTP response functionality needed here
    
    // Helper function to convert status code to string
    inline sep::shim::string status_code_to_string(int status_code) {
        switch (status_code) {
            case 200: return "OK";
            case 201: return "Created";
            case 204: return "No Content";
            case 400: return "Bad Request";
            case 401: return "Unauthorized";
            case 403: return "Forbidden";
            case 404: return "Not Found";
            case 500: return "Internal Server Error";
            default: return "Unknown";
        }
    }
}