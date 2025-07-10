#pragma once
#define SEP_CROW_ISOLATION_INCLUDED

// This header provides Crow compatibility without requiring the real Crow implementation
#include <http_parser.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

// Define routing_params to fix compilation issues
struct routing_params
{
};

#ifdef __cplusplus
namespace crow
{
    // Use std::string directly
    using string = std::string;
    using crow_string = std::string;

    enum class HTTPMethod
    {
        DELETE,
        GET,
        HEAD,
        POST,
        PUT,
        CONNECT,
        OPTIONS,
        TRACE,
        PATCH,
        PURGE
    };

    // Required method_name function
    inline const char* method_name(HTTPMethod method)
    {
        static const char* method_names[] = {"DELETE",  "GET",     "HEAD",  "POST",  "PUT",
                                             "CONNECT", "OPTIONS", "TRACE", "PATCH", "PURGE"};
        return method_names[static_cast<int>(method)];
    }

// Status codes enum for easier use (only define if not already defined)
#ifndef CROW_STATUS_CODES_DEFINED
#define CROW_STATUS_CODES_DEFINED
    enum class status
    {
        OK = 200,
        CREATED = 201,
        ACCEPTED = 202,
        NO_CONTENT = 204,
        BAD_REQUEST = 400,
        UNAUTHORIZED = 401,
        FORBIDDEN = 403,
        NOT_FOUND = 404,
        INTERNAL_SERVER_ERROR = 500,
        SERVICE_UNAVAILABLE = 503
    };
#endif

    // Stub for request class
    class request {
    public:
        HTTPMethod method;
        std::string url;
        std::string body;
        std::map<std::string, std::string> headers;

        request() : method(HTTPMethod::GET) {}

        const char* get_header_value(const std::string& key) const
        {
            auto it = headers.find(key);
            if (it != headers.end()) return it->second.c_str();
            static const char* empty = "";
            return empty;
        }

        const char* get_remote_ip() const { return "127.0.0.1"; }
    };

    // Stub for response class
    class response {
    public:
        int code;
        int status;
        std::string body;
        std::map<std::string, std::string> headers;

        response() : code(200), status(200) {}
        explicit response(int c) : code(c), status(c) {}

        void set_header(const std::string& key, const std::string& value) { headers[key] = value; }

        void add_header(const std::string& key, const std::string& value) { headers[key] = value; }

        void write(const std::string& data) { body += data; }

        void end() {}
    };

    // Minimal context implementation
    template<typename Adaptor, typename Handler, typename... Middlewares>
    class context {
    public:
        request req;
        response res;
    };

    // Define a basic Crow application template class to fix compilation issues
    template <typename... Middlewares>
    class Crow
    {
    public:
        Crow() {}

        void port(int p) { port_ = p; }
        void multithreaded() { is_multithreaded_ = true; }
        void run() {}
        void stop() {}

        template <typename MW>
        MW& get_middleware()
        {
            static MW mw;
            return mw;
        }

        // Define the RouteHandler class with required functionality
        class RouteHandler
        {
        public:
            RouteHandler() {}

            // Add methods function to support the call chains in server.cpp
            RouteHandler& methods(HTTPMethod method)
            {
                (void)method;  // Avoid unused parameter warning
                return *this;
            }

            // Add operator() to handle lambda registration
            template <typename Func>
            void operator()(Func f)
            {
                (void)f;  // Avoid unused parameter warning
            }
        };

        // Method to create a route
        RouteHandler route_dynamic(const std::string& url)
        {
            (void)url;  // Avoid unused parameter warning
            return RouteHandler();
        }

    private:
        int port_ = 8080;
        bool is_multithreaded_ = false;
    };
}  // namespace crow
#endif
