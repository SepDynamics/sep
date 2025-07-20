#pragma once
#define SEP_CROW_ISOLATION_INCLUDED

// This header provides Crow compatibility without requiring the real Crow implementation
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

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
    // Use shim::string directly
    using string = shim::string;
    using crow_string = shim::string;

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
        shim::string url;
        shim::string body;
        std::map<shim::string, shim::string> headers;

        request() : method(HTTPMethod::GET) {}

        const char* get_header_value(const shim::string& key) const
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
        shim::string body;
        std::map<shim::string, shim::string> headers;

        response() : code(200), status(200) {}
        explicit response(int c) : code(c), status(c) {}

        void set_header(const shim::string& key, const shim::string& value)
        {
            headers[key] = value;
        }

        void add_header(const shim::string& key, const shim::string& value)
        {
            headers[key] = value;
        }

        void write(const shim::string& data) { body += data; }

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
        void run()
        {
            should_stop_ = false;
            server_fd_ = ::socket(AF_INET, SOCK_STREAM, 0);
            if (server_fd_ < 0)
                return;

            int opt = 1;
            ::setsockopt(server_fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

            sockaddr_in addr{};
            addr.sin_family = AF_INET;
            addr.sin_addr.s_addr = htonl(INADDR_ANY);
            addr.sin_port = htons(static_cast<uint16_t>(port_));

            if (::bind(server_fd_, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0)
            {
                ::close(server_fd_);
                server_fd_ = -1;
                return;
            }

            ::listen(server_fd_, 16);

            while (!should_stop_)
            {
                int client_fd = ::accept(server_fd_, nullptr, nullptr);
                if (client_fd < 0)
                    continue;

                const char response[] = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
                ::send(client_fd, response, sizeof(response) - 1, 0);
                ::close(client_fd);
            }

            ::close(server_fd_);
            server_fd_ = -1;
        }

        void stop()
        {
            should_stop_ = true;
            if (server_fd_ != -1)
            {
                ::shutdown(server_fd_, SHUT_RDWR);
            }
        }

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
        RouteHandler route_dynamic(const shim::string& url)
        {
            (void)url;  // Avoid unused parameter warning
            return RouteHandler();
        }

    private:
        int port_ = 3000;
        bool is_multithreaded_ = false;
        int server_fd_ = -1;
        bool should_stop_ = false;
    };
}  // namespace crow
#endif
