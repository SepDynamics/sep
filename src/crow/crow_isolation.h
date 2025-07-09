#pragma once
#define SEP_CROW_ISOLATION_INCLUDED

// This header isolates Crow-related code from CUDA compilation
#include <http_parser.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Forward declare the crow namespace and required classes
// This makes ::crow references work
#ifdef __cplusplus
namespace crow
{
    // Define a simple string class for isolation
    class crow_string
    {
    public:
        crow_string() {}
        crow_string(const char* /* s */) {}
        const char* c_str() const { return ""; }
    };

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
    inline const char* method_name(HTTPMethod /* method */)
    {
        static const char* empty = "";
        return empty;
    }

    // Stub for request class
    class request {
    public:
        HTTPMethod method;
        crow_string url;
        crow_string body;

        request() : method(HTTPMethod::GET) {}

        const char* get_header_value(const crow_string& /* key */) const
        {
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
        crow_string body;

        response() : code(200), status(200) {}
        explicit response(int c) : code(c), status(c) {}

        void set_header(const crow_string& /* key */, const crow_string& /* value */) {}
        void add_header(const crow_string& /* key */, const crow_string& /* value */) {}
        void write(const crow_string& /* data */) {}
        void end() {}
    };

    // Minimal context implementation
    template<typename Adaptor, typename Handler, typename... Middlewares>
    class context {
    public:
        request req;
        response res;
    };
}  // namespace crow
#endif
