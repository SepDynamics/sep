#pragma once

// Socket adaptor utilities used by the Crow HTTP server.  The original project
// provided a stripped down stub to avoid depending on Boost.ASIO.  We now rely
// directly on Boost.ASIO via the aliases defined in `asio_isolation.h`.

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

// Include our own headers
#include "compat/shim.h"
#include "crow/logging.h"
#include "crow/asio_isolation.h"

namespace crow {
    // TCP alias from Boost.ASIO
    using tcp = asio_stub::ip::tcp;

    /// A wrapper for the asio::ip::tcp::socket
    struct SocketAdaptor {
        using context = void;
        explicit SocketAdaptor(asio_stub::io_context& io_context, context*)
            : socket_(io_context) {}

        asio_stub::io_context& get_io_context() {
            return static_cast<asio_stub::io_context&>(
                socket_.get_executor().context());
        }

        tcp::socket& raw_socket() { return socket_; }
        tcp::socket& socket() { return socket_; }
        tcp::endpoint remote_endpoint() {
            error_code ec;
            return socket_.remote_endpoint(ec);
        }
        bool is_open() { return socket_.is_open(); }

        error_code close() {
            error_code ec;
            socket_.close(ec);
            return ec;
        }

        error_code shutdown_readwrite() {
            error_code ec;
            socket_.shutdown(asio_stub::socket_base::shutdown_both, ec);
            return ec;
        }

        error_code shutdown_write() {
            error_code ec;
            socket_.shutdown(asio_stub::socket_base::shutdown_send, ec);
            return ec;
        }

        error_code shutdown_read() {
            error_code ec;
            socket_.shutdown(asio_stub::socket_base::shutdown_receive, ec);
            return ec;
        }

        template <typename F>
        void start(F f) {
            f(error_code());
        }

        tcp::socket socket_;
    };

    #ifdef CROW_ENABLE_SSL
    struct SSLAdaptor {
        using context = asio_stub::ssl::context;
        using ssl_socket_t = asio_stub::ssl::stream<tcp::socket>;
        
        SSLAdaptor(asio_stub::io_context& io_context, context* ctx)
            : ssl_socket_(new ssl_socket_t(io_context, *ctx)) {}

        ssl_socket_t& socket() { return *ssl_socket_; }
        tcp::socket& raw_socket() { return ssl_socket_->next_layer(); }
        tcp::endpoint remote_endpoint() {
            error_code ec;
            return raw_socket().remote_endpoint(ec);
        }
        bool is_open() { return ssl_socket_ ? raw_socket().is_open() : false; }

        error_code close() {
            error_code ec;
            if (is_open()) {
                raw_socket().close(ec);
            }
            return ec;
        }

        error_code shutdown_readwrite() {
            error_code ec;
            if (is_open()) {
                raw_socket().shutdown(asio_stub::socket_base::shutdown_both, ec);
            }
            return ec;
        }

        error_code shutdown_write() {
            error_code ec;
            if (is_open()) {
                raw_socket().shutdown(asio_stub::socket_base::shutdown_send, ec);
            }
            return ec;
        }

        error_code shutdown_read() {
            error_code ec;
            if (is_open()) {
                raw_socket().shutdown(asio_stub::socket_base::shutdown_receive, ec);
            }
            return ec;
        }

        asio_stub::io_context& get_io_context() {
            return raw_socket().get_executor().context();
        }

        template <typename F>
        void start(F f) {
            ssl_socket_->async_handshake(asio_stub::ssl::server, [f](const error_code& ec) { f(ec); });
        }

        std::unique_ptr<ssl_socket_t> ssl_socket_;
    };
    #endif
}  // namespace crow