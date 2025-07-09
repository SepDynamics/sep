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
namespace std
{
    class string
    {
    public:
        string() {}
        string(const char* s) {}
        const char* c_str() const { return ""; }
    };
}  // namespace std

namespace crow
{
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

    // Stub for request class
    class request {
    public:
        HTTPMethod method;
        std::string url;
        std::string body;

        request() : method(HTTPMethod::GET) {}

        const char* get_header_value(const std::string& key) const {
            static const char* empty = "";
            return empty;
        }
    };

    // Stub for response class
    class response {
    public:
        int code;
        int status;
        std::string body;

        response() : code(200), status(200) {}
        explicit response(int c) : code(c), status(c) {}

        void set_header(const std::string& key, const std::string& value) {}
        void add_header(const std::string& key, const std::string& value) {}
        void write(const std::string& data) {}
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
