#pragma once

#define SEP_CROW_ISOLATION_INCLUDED

// This header file is used to isolate Crow-related code from CUDA compilation
// It provides stub implementations for Crow functionality that can be
// safely included in CUDA files without causing template instantiation errors

// Include our own headers
#include "compat/shim.h"
#include "common.h"

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

// Crow namespace with stub implementations
namespace crow {
    // Stub implementation for request
    class request {
    public:
        HTTPMethod  method;
        sep::shim::string url;
        sep::shim::string body;

        const char* get_header_value(const sep::shim::string& key) const {
            return "";
        }
        
        // Default constructor - IMPORTANT: Use HTTPMethod::GET (uppercase) to match original Crow
        request() : method(HTTPMethod::GET) {}
    };

    // Stub implementation for response
    class response {
    public:
        int         code{200};
        int         status{200};
        sep::shim::string body;

        response() = default;
        explicit response(int c) : code(c), status(c) {}

        void set_header(const sep::shim::string& key, const sep::shim::string& value) {}
        void add_header(const sep::shim::string& key, const sep::shim::string& value) {}
        void write(const sep::shim::string& data) {
            body = data;
        }
        void end() {}
    };

    // Stub implementation for websocket
    namespace websocket {
        enum class connection_state {
            open,
            closing,
            closed
        };

        class connection {
        public:
            void send_text(const sep::shim::string& text) {}
            void send_binary(const sep::shim::string& data) {}
            void close(const sep::shim::string& msg = "") {}

            connection_state get_state() const {
                return connection_state::closed;
            }
        };
    }  // namespace websocket

    // Stub implementation for middleware context
    template<typename Adaptor, typename Handler, typename... Middlewares>
    class context {
    public:
        request  req;
        response res;
    };

    // Minimal stub representing a route rule returned by CROW_ROUTE and friends
    class DummyRoute {
    public:
        template<typename... Args>
        DummyRoute& methods(Args&&...) {
            return *this;
        }

        template<typename F>
        DummyRoute& operator()(F&& f) {
            return *this;
        }

        template<typename... Args>
        DummyRoute& websocket(Args&&...) {
            return *this;
        }
    };

    // Non-templated base class for Crow app to avoid template issues
    class CrowBase {
    public:
        CrowBase() {}

        void port(uint16_t p) {}
        void bindaddr(const sep::shim::string& addr) {}
        void multithreaded() {}
        void run() {}
        void stop() {}

        DummyRoute route(const sep::shim::string&) {
            return {};
        }

        DummyRoute route_dynamic(const sep::shim::string&) {
            return {};
        }

        DummyRoute catchall_route() {
            return {};
        }
    };

    // Forward declaration of the templated Crow class
    template<typename... Middlewares>
    class Crow;
    
    // Forward declaration of the templated crow class (lowercase for backward compatibility)
    template<typename... Middlewares>
    class crow;
    
    // Templated Crow app that inherits from the non-templated base
    template<typename... Middlewares>
    class Crow : public CrowBase {
    public:
        Crow() : CrowBase() {}

        template<typename T>
        T& get_middleware() {
            // Stub implementation that returns a default-constructed T
            static T middleware;
            return middleware;
        }

        template<typename Adaptor>
        void handle_upgrade(const request& req,
                            response& res,
                            Adaptor&& adaptor) {}
    };
    
    // Lowercase version with same implementation for backward compatibility
    template<typename... Middlewares>
    class crow : public CrowBase {
    public:
        crow() : CrowBase() {}

        template<typename T>
        T& get_middleware() {
            // Stub implementation that returns a default-constructed T
            static T middleware;
            return middleware;
        }

        template<typename Adaptor>
        void handle_upgrade(const request& req,
                            response& res,
                            Adaptor&& adaptor) {}
    };

    // Stub implementation for Crow app (alias)
    template<typename... Middlewares>
    using app = Crow<Middlewares...>;

    using simpleApp = crow<>;

    // Minimal replacements for Crow convenience macros
    #define CROW_ROUTE(app, url) app.route(url)
    #define CROW_BP_ROUTE(bp, url) bp.route(url)
    #define CROW_WEBSOCKET_ROUTE(app, url) app.route(url).websocket(&app)
    #define CROW_MIDDLEWARES(app, ...) .middlewares(__VA_ARGS__)
    #define CROW_CATCHALL_ROUTE(app) app.catchall_route()
    #define CROW_BP_CATCHALL_ROUTE(bp) bp.catchall_rule()

    // Use the official http-parser types inside a dedicated namespace to keep
    // compatibility with code that expects crow::http_parser_stub symbols.
    namespace http_parser_stub {
        using http_errno = ::http_errno;
        using http_parser_type = ::http_parser_type;
        using http_parser = ::http_parser;
        using http_parser_settings = ::http_parser_settings;
        using http_parser_url = ::http_parser_url;
    }  // namespace http_parser_stub
}  // namespace crow
