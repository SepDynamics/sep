#pragma once

// When not compiling with NVCC, include the real Crow common header directly.
#ifndef __CUDACC__
#include <crow/common.h>
#else

// This is a minimal version of the common.h file from the Crow framework
// used during CUDA compilation to avoid heavy template instantiation.

// Include our own headers
#include "compat/shim.h"

// Define CROW_LIKELY and CROW_UNLIKELY if not already defined
#ifndef CROW_LIKELY
#if defined(__GNUG__) || defined(__clang__)
#define CROW_LIKELY(X) __builtin_expect(!!(X), 1)
#define CROW_UNLIKELY(X) __builtin_expect(!!(X), 0)
#else
#define CROW_LIKELY(X) (X)
#define CROW_UNLIKELY(X) (X)
#endif
#endif

namespace crow
 {
    const char cr = '\r';
    const char lf = '\n';
    const sep::shim::string crlf("\r\n");

    // Stub implementation for HTTP methods
    // IMPORTANT: Use uppercase names to match the original Crow implementation
    enum class HTTPMethod : char {
        DELETE = 0,
        GET,
        HEAD,
        POST,
        PUT,

        CONNECT,
        OPTIONS,
        TRACE,

        PATCH,
        PURGE,

        COPY,
        LOCK,
        MKCOL,
        MOVE,
        PROPFIND,
        PROPPATCH,
        SEARCH,
        UNLOCK,
        BIND,
        REBIND,
        UNBIND,
        ACL,

        REPORT,
        MKACTIVITY,
        CHECKOUT,
        MERGE,

        MSEARCH,
        NOTIFY,
        SUBSCRIBE,
        UNSUBSCRIBE,

        MKCALENDAR,

        LINK,
        UNLINK,

        SOURCE,

        InternalMethodCount,
    };

    inline sep::shim::string method_name(HTTPMethod method) {
        switch (method) {
            case HTTPMethod::GET:
                return "GET";
            case HTTPMethod::POST:
                return "POST";
            case HTTPMethod::PUT:
                return "PUT";
            case HTTPMethod::DELETE:
                return "DELETE";
            case HTTPMethod::HEAD:
                return "HEAD";
            case HTTPMethod::OPTIONS:
                return "OPTIONS";
            case HTTPMethod::PATCH:
                return "PATCH";
            case HTTPMethod::CONNECT:
                return "CONNECT";
            case HTTPMethod::TRACE:
                return "TRACE";
            default:
                return "invalid";
        }
    }

    // Stub implementation for HTTP status codes
    enum class status {
        OK                            = 200,
        CREATED                       = 201,
        ACCEPTED                      = 202,
        NO_CONTENT                    = 204,
        RESET_CONTENT                 = 205,
        PARTIAL_CONTENT               = 206,
        MULTIPLE_CHOICES              = 300,
        MOVED_PERMANENTLY             = 301,
        FOUND                         = 302,
        SEE_OTHER                     = 303,
        NOT_MODIFIED                  = 304,
        TEMPORARY_REDIRECT            = 307,
        PERMANENT_REDIRECT            = 308,
        BAD_REQUEST                   = 400,
        UNAUTHORIZED                  = 401,
        FORBIDDEN                     = 403,
        NOT_FOUND                     = 404,
        METHOD_NOT_ALLOWED            = 405,
        PROXY_AUTHENTICATION_REQUIRED = 407,
        CONFLICT                      = 409,
        GONE                          = 410,
        PAYLOAD_TOO_LARGE             = 413,
        UNSUPPORTED_MEDIA_TYPE        = 415,
        RANGE_NOT_SATISFIABLE         = 416,
        EXPECTATION_FAILED            = 417,
        PRECONDITION_REQUIRED         = 428,
        TOO_MANY_REQUESTS             = 429,
        UNAVAILABLE_FOR_LEGAL_REASONS = 451,
        INTERNAL_SERVER_ERROR         = 500,
        NOT_IMPLEMENTED               = 501,
        BAD_GATEWAY                   = 502,
        SERVICE_UNAVAILABLE           = 503,
        GATEWAY_TIMEOUT               = 504,
        VARIANT_ALSO_NEGOTIATES       = 506
    };

    // Stub implementation for routing_handle_result
    struct routing_handle_result {
        routing_handle_result() {}
    };

} // namespace crow
#endif
