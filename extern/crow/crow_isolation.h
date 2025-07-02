#pragma once

#define SEP_CROW_ISOLATION_INCLUDED

// This header file is used to isolate Crow-related code from CUDA compilation
// It provides stub implementations for Crow functionality that can be
// safely included in CUDA files without causing template instantiation errors

// Use minimal stubs when the standard library is unavailable (e.g. CUDA builds).
#if defined(SEP_NO_STDLIB) || defined(__CUDACC__)
#    define SEP_HAS_STDLIB 0
#    include <stdlib.h>
#    include <string.h>
// Forward declare shim types - actual implementation is in shim.h
namespace sep {
namespace shim {
class string;  // Forward declaration only
}  // namespace shim
}  // namespace sep
#else
#    define SEP_HAS_STDLIB 1
#    include <nlohmann/json.hpp>
#    include <stdexcept>
#    include <string>
#endif

namespace crow {
#if !defined(__cpp_exceptions) && !defined(__EXCEPTIONS) && !defined(_CPPUNWIND)
extern const char* last_error;
#endif
}  // namespace crow

#ifndef CROW_RAISE
#    if defined(__cpp_exceptions) || defined(__EXCEPTIONS) || defined(_CPPUNWIND)
#        define CROW_RAISE(msg) throw std::runtime_error(msg)
#    else
#        define CROW_RAISE(msg)         \
            do                          \
            {                           \
                crow::last_error = msg; \
                std::abort();           \
            }                           \
            while (0)
#    endif
#endif

// Check for CUDA compilation or absence of the full Crow headers
// We also fall back to the stubs when RTTI is disabled or <crow/app.h> cannot be found
#if defined(__CUDACC__) || defined(SEP_CUDA_COMPILATION) || defined(CROW_DISABLE_RTTI) \
    || !(__has_include(<crow/app.h>) && __has_include(<boost/asio.hpp>))
// When compiling with CUDA or Crow is unavailable, provide stub implementations
#    ifndef SEP_FULL_CROW_AVAILABLE
#        define SEP_FULL_CROW_AVAILABLE 0
#    endif
#    ifndef SEP_CUDA_COMPILATION
#        define SEP_CUDA_COMPILATION
#    endif

// Define ASIO_STANDALONE before including asio_isolation.h
#    ifndef ASIO_STANDALONE
#        define ASIO_STANDALONE
#    endif

// Include our ASIO isolation header
#    include "crow/asio_isolation.h"

// Include our std isolation header for string and system_error
#    include "compat/shim.h"

// Include standard headers needed for stubs
#    include <cstdint>
#    include <cstring>

// Define HTTP parser constants
#    define HTTP_PARSER_VERSION_MAJOR 2
#    define HTTP_PARSER_VERSION_MINOR 9

// Crow namespace with stub implementations
namespace crow {
// Forward declarations for Crow classes
class request;
class response;

// Stub implementation for HTTP methods
enum class HTTPMethod : char
{
    GET,
    POST,
    PUT,
    DELETE,
    HEAD,
    OPTIONS,
    PATCH,
    CONNECT,
    TRACE,
    PURGE,
    COPY,
    LOCK,
    MKCOL,
    MOVE,
    InternalMethodCount
};

inline const char* method_name(HTTPMethod method)
{
    switch (method)
    {
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
enum class status
{
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

// Stub implementation for request
class request
{
public:
    HTTPMethod  method;
    std::string url;
    std::string body;

    const char* get_header_value(const std::string& key [[maybe_unused]]) const
    {
        (void)key;
        return "";
    }
};

// Stub implementation for response
class response
{
public:
    int         code{200};
    int         status{200};
    std::string body;

    response() = default;
    explicit response(int c) : code(c), status(c) {}

    void set_header(const std::string& key [[maybe_unused]], const std::string& value [[maybe_unused]])
    {
        (void)key;
        (void)value;
    }
    void add_header(const std::string& key [[maybe_unused]], const std::string& value [[maybe_unused]])
    {
        (void)key;
        (void)value;
    }
    void write(const std::string& data [[maybe_unused]])
    {
        body = data;
    }
    void write_json(const nlohmann::json& j)
    {
        body = sep::shim::string(j.dump().c_str());
    }
    void end() {}
};

// Stub implementation for websocket
namespace websocket {
enum class connection_state
{
    open,
    closing,
    closed
};

class connection
{
public:
    void send_text(const std::string& text [[maybe_unused]])
    {
        (void)text;
    }
    void send_binary(const std::string& data [[maybe_unused]])
    {
        (void)data;
    }
    void close(const std::string& msg [[maybe_unused]] = "")
    {
        (void)msg;
    }

    connection_state get_state() const
    {
        return connection_state::closed;
    }
};
}  // namespace websocket

// Stub implementation for middleware context
template<typename Adaptor, typename Handler, typename... Middlewares>
class context
{
public:
    request  req;
    response res;
};

// Stub implementation for routing
class routing_handle_result
{
public:
    routing_handle_result() {}
};

// Minimal stub representing a route rule returned by CROW_ROUTE and friends
class DummyRoute
{
public:
    template<typename... Args>
    DummyRoute& methods([[maybe_unused]] Args&&...)
    {
        return *this;
    }

    template<typename F>
    DummyRoute& operator()(F&& f [[maybe_unused]])
    {
        return *this;
    }

    template<typename... Args>
    DummyRoute& websocket([[maybe_unused]] Args&&...)
    {
        return *this;
    };
};

// Stub implementation for TCP adaptors
namespace socket_adaptors {
class tcp
{
public:
    using endpoint = void*;
    using socket   = void*;

    tcp(asio::io_context& io_service) {}
};
}  // namespace socket_adaptors

// Stub implementation for Crow app
template<typename... Middlewares>
class Crow
{
public:
    Crow() {}

    void port(std::uint16_t p [[maybe_unused]])
    {
        (void)p;
    }
    void bindaddr(const std::string& addr [[maybe_unused]])
    {
        (void)addr;
    }
    void multithreaded() {}
    void run() {}
    void stop() {}

    DummyRoute route([[maybe_unused]] const std::string&)
    {
        return {};
    }

    DummyRoute route_dynamic([[maybe_unused]] const std::string&)
    {
        return {};
    }

    DummyRoute catchall_route()
    {
        return {};
    }

    template<typename Adaptor>
    void handle_upgrade([[maybe_unused]] const request& req,
                        [[maybe_unused]] response&      res,
                        [[maybe_unused]] Adaptor&&      adaptor)
    {
        (void)req;
        (void)res;
        (void)adaptor;
    }
};

// Stub implementation for Crow app (alias)
template<typename... Middlewares>
using App = Crow<Middlewares...>;

using SimpleApp = Crow<>;

// Minimal replacements for Crow convenience macros
#    define CROW_ROUTE(app, url) app.route(url)
#    define CROW_BP_ROUTE(bp, url) bp.route(url)
#    define CROW_WEBSOCKET_ROUTE(app, url) app.route(url).websocket(&app)
#    define CROW_MIDDLEWARES(app, ...) .middlewares(__VA_ARGS__)
#    define CROW_CATCHALL_ROUTE(app) app.catchall_route()
#    define CROW_BP_CATCHALL_ROUTE(bp) bp.catchall_rule()

inline HTTPMethod operator"" _method(const char* str, size_t)
{
    if (std::strcmp(str, "POST") == 0)
        return HTTPMethod::POST;
    if (std::strcmp(str, "PUT") == 0)
        return HTTPMethod::PUT;
    if (std::strcmp(str, "DELETE") == 0)
        return HTTPMethod::DELETE;
    return HTTPMethod::GET;
}

// HTTP parser stubs
namespace http_parser {
enum class http_errno
{
    HPE_OK,
    HPE_CB_message_begin,
    HPE_CB_url,
    HPE_CB_header_field,
    HPE_CB_header_value,
    HPE_CB_headers_complete,
    HPE_CB_body,
    HPE_CB_message_complete,
    HPE_CB_status,
    HPE_CB_chunk_header,
    HPE_CB_chunk_complete,
    HPE_INVALID_EOF_STATE,
    HPE_HEADER_OVERFLOW,
    HPE_CLOSED_CONNECTION,
    HPE_INVALID_VERSION,
    HPE_INVALID_STATUS,
    HPE_INVALID_METHOD,
    HPE_INVALID_URL,
    HPE_INVALID_HOST,
    HPE_INVALID_PORT,
    HPE_INVALID_PATH,
    HPE_INVALID_QUERY_STRING,
    HPE_INVALID_FRAGMENT,
    HPE_LF_EXPECTED,
    HPE_INVALID_HEADER_TOKEN,
    HPE_INVALID_CONTENT_LENGTH,
    HPE_UNEXPECTED_CONTENT_LENGTH,
    HPE_INVALID_CHUNK_SIZE,
    HPE_INVALID_CONSTANT,
    HPE_INVALID_INTERNAL_STATE,
    HPE_STRICT,
    HPE_PAUSED,
    HPE_UNKNOWN
};

enum class http_parser_type
{
    HTTP_REQUEST,
    HTTP_RESPONSE,
    HTTP_BOTH
};

struct http_parser
{
    unsigned int   type : 2;
    unsigned int   flags : 8;
    unsigned int   state : 8;
    unsigned int   header_state : 8;
    unsigned int   index : 8;
    std::uint32_t  nread;
    std::uint64_t  content_length;
    unsigned short http_major;
    unsigned short http_minor;
    unsigned int   status_code : 16;
    unsigned int   method : 8;
    unsigned int   http_errno : 7;
    unsigned int   upgrade : 1;
    void*          data;
};

struct http_parser_settings
{
    void* on_message_begin;
    void* on_url;
    void* on_status;
    void* on_header_field;
    void* on_header_value;
    void* on_headers_complete;
    void* on_body;
    void* on_message_complete;
    void* on_chunk_header;
    void* on_chunk_complete;
};
}  // namespace http_parser
}  // namespace crow

namespace sep {
}

#else
#    ifndef SEP_FULL_CROW_AVAILABLE
#        define SEP_FULL_CROW_AVAILABLE 1
#    endif
#    include "crow/app.h"
#    include "crow/parser.h"
namespace sep {
namespace crow {
using ::crow::request;
using ::crow::response;
using ::crow::routing_handle_result;

template<typename... Middlewares>
using Crow = ::crow::Crow<Middlewares...>;

template<typename... Middlewares>
using App = ::crow::App<Middlewares...>;

using SimpleApp = ::crow::SimpleApp;

namespace black_magic = ::crow::black_magic;
namespace websocket   = ::crow::websocket;

namespace http_parser {
using ::crow::http_parser;
using ::crow::http_parser_settings;
using http_errno = ::crow::http_errno;
}

}  // namespace crow
}  // namespace sep
#endif
