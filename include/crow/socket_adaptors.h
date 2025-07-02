#pragma once

// This is a minimal stub version of socket_adaptors.h for the Crow framework
// It provides just enough to compile without errors, but doesn't implement full functionality

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
    // Alias Boost.ASIO TCP type
    using tcp = asio_stub::ip::tcp;

    /// A wrapper for the asio::ip::tcp::socket
    struct SocketAdaptor {
        using context = void;
        explicit SocketAdaptor(asio_stub::io_context& io_context, context*)
            : socket_(io_context), io_(io_context) {}

        asio_stub::io_context& get_io_context() { return io_; }

        tcp::socket& raw_socket() { return socket_; }
        tcp::socket& socket() { return socket_; }
        tcp::endpoint remote_endpoint() {
            error_code ec;
            auto ep = socket_.remote_endpoint(ec);
            return ec ? tcp::endpoint() : ep;
        }
        bool is_open() { return socket_.is_open(); }

        error_code close() {
            error_code ec;
            if (!is_open()) {
                ec = make_error_code(asio_stub::error::not_connected);
                return ec;
            }
            socket_.close(ec);
            return ec;
        }

        error_code shutdown_readwrite() {
            error_code ec;
            socket_.shutdown(tcp::socket::shutdown_both, ec);
            return ec;
        }

        error_code shutdown_write() {
            error_code ec;
            socket_.shutdown(tcp::socket::shutdown_send, ec);
            return ec;
        }

        error_code shutdown_read() {
            error_code ec;
            socket_.shutdown(tcp::socket::shutdown_receive, ec);
            return ec;
        }

        template <typename F>
        void start(F f) {
            f(error_code());
        }

        tcp::socket socket_;
        asio_stub::io_context& io_;
    };

    #ifdef CROW_ENABLE_SSL
    struct SSLAdaptor {
        using context = asio_stub::ssl::context;
        using ssl_socket_t = asio_stub::ssl::stream<tcp::socket>;

        SSLAdaptor(asio_stub::io_context& io_context, context* ctx)
            : ssl_socket_(std::make_unique<ssl_socket_t>(io_context, *ctx)) {}

        ssl_socket_t& socket() { return *ssl_socket_; }
        tcp::socket& raw_socket() { return ssl_socket_->next_layer(); }
        tcp::endpoint remote_endpoint() {
            error_code ec;
            auto ep = raw_socket().remote_endpoint(ec);
            return ec ? tcp::endpoint() : ep;
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
                raw_socket().shutdown(tcp::socket::shutdown_both, ec);
            }
            return ec;
        }

        error_code shutdown_write() {
            error_code ec;
            if (is_open()) {
                raw_socket().shutdown(tcp::socket::shutdown_send, ec);
            }
            return ec;
        }

        error_code shutdown_read() {
            error_code ec;
            if (is_open()) {
                raw_socket().shutdown(tcp::socket::shutdown_receive, ec);
            }
            return ec;
        }

        asio_stub::io_context& get_io_context() {
            return ssl_socket_->get_executor().context();
        }

        template <typename F>
        void start(F f) {
            ssl_socket_->async_handshake(asio_stub::ssl::stream_base::server, [f](const error_code& ec) { f(ec); });
        }

        std::unique_ptr<ssl_socket_t> ssl_socket_;
    };
    #endif
}  // namespace crow