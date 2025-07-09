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

// Include our own headers and Boost.ASIO
#include "compat/shim.h"
#include "crow/logging.h"
#include "crow/asio_isolation.h"
#include <boost/asio.hpp>
#ifdef CROW_ENABLE_SSL
#include <boost/asio/ssl.hpp>
#endif

namespace crow {
    // TCP alias from Boost.ASIO
    using tcp = asio_stub::ip::tcp;

    /// A wrapper for the asio::ip::tcp::socket
    struct SocketAdaptor {
        using context = void;
        explicit SocketAdaptor(asio_stub::io_context& io_context, context*)
            : socket_(io_context), io_(io_context) {}

        asio_stub::io_context& get_io_context() {
            return io_;
        }

        tcp::socket& raw_socket() { return socket_; }
        tcp::socket& socket() { return socket_; }
        tcp::endpoint remote_endpoint() {
            asio_stub::error_code ec;
            return socket_.remote_endpoint(ec);
        }
        bool is_open() { return socket_.is_open(); }

        error_t close()
        {
            error_t ec = 0;
            if (!is_open()) {
                // Use a simple integer error code since error_t is an integer
                ec = asio_stub::error::not_connected;
                return ec;
            }
            asio_stub::error_code asio_ec;
            socket_.close(asio_ec);
            ec = asio_ec ? asio_ec.value() : 0;
            return ec;
        }

        error_t shutdown_readwrite()
        {
            error_t ec = 0;
            asio::error_code asio_ec;
            socket_.shutdown(tcp::socket::shutdown_both, asio_ec);
            ec = asio_ec ? asio_ec.value() : 0;
            return ec;
        }

        error_t shutdown_write()
        {
            error_t ec = 0;
            asio::error_code asio_ec;
            socket_.shutdown(tcp::socket::shutdown_send, asio_ec);
            ec = asio_ec ? asio_ec.value() : 0;
            return ec;
        }

        error_t shutdown_read()
        {
            error_t ec = 0;
            asio::error_code asio_ec;
            socket_.shutdown(tcp::socket::shutdown_receive, asio_ec);
            ec = asio_ec ? asio_ec.value() : 0;
            return ec;
        }

        template <typename F>
        void start(F f) {
            f(error_t{});
        }

        tcp::socket socket_;
        asio_stub::io_context& io_;
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
            error_t ec;
            return raw_socket().remote_endpoint(ec);
        }
        bool is_open() { return ssl_socket_ ? raw_socket().is_open() : false; }

        error_t close()
        {
            error_t ec;
            if (!is_open()) {
                ec = asio::error::not_connected;
            } else {
                raw_socket().close(ec);
            }
            return ec;
        }

        error_t shutdown_readwrite()
        {
            error_t ec;
            if (is_open()) {
                raw_socket().shutdown(tcp::socket::shutdown_both, ec);
            }
            return ec;
        }

        error_t shutdown_write()
        {
            error_t ec;
            if (is_open()) {
                raw_socket().shutdown(tcp::socket::shutdown_send, ec);
            }
            return ec;
        }

        error_t shutdown_read()
        {
            error_t ec;
            if (is_open()) {
                raw_socket().shutdown(tcp::socket::shutdown_receive, ec);
            }
            return ec;
        }

        asio_stub::io_context& get_io_context() {
            return raw_socket().get_executor().context();
        }

        template <typename F>
        void start(F f) {
            ssl_socket_->async_handshake(asio_stub::ssl::stream_base::server,
                                         [f](const error_t& ec) { f(ec); });
        }

        std::unique_ptr<ssl_socket_t> ssl_socket_;
    };
    #endif
}  // namespace crow
