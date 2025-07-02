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

// Include our own headers
#include "compat/shim.h"
#include "crow/logging.h"
#include "crow/asio_isolation.h"
#include <memory>

namespace crow {
    using tcp = asio::ip::tcp;

    /// A wrapper for the asio::ip::tcp::socket
    struct SocketAdaptor {
        using context = void;
        explicit SocketAdaptor(asio::io_context& io_context, context*) : socket_(io_context), io_ctx_(&io_context) {
            boost::system::error_code ignore_ec;
            socket_.close(ignore_ec);
        }

        asio::io_context& get_io_context() { return *io_ctx_; }

        tcp::socket& raw_socket() { return socket_; }
        tcp::socket& socket() { return socket_; }
        tcp::endpoint remote_endpoint() { return socket_.remote_endpoint(); }
        bool is_open() { return socket_.is_open(); }

        error_code close() {
            error_code ec;
            if (!socket_.is_open()) {
                ec = boost::asio::error::not_connected;
            } else {
                socket_.close(ec);
            }
            return ec;
        }

        error_code shutdown_readwrite() {
            error_code ec;
            if (!socket_.is_open()) {
                ec = boost::asio::error::not_connected;
            } else {
                socket_.shutdown(boost::asio::socket_base::shutdown_both, ec);
            }
            return ec;
        }

        error_code shutdown_write() {
            error_code ec;
            if (!socket_.is_open()) {
                ec = boost::asio::error::not_connected;
            } else {
                socket_.shutdown(boost::asio::socket_base::shutdown_send, ec);
            }
            return ec;
        }

        error_code shutdown_read() {
            error_code ec;
            if (!socket_.is_open()) {
                ec = boost::asio::error::not_connected;
            } else {
                socket_.shutdown(boost::asio::socket_base::shutdown_receive, ec);
            }
            return ec;
        }

        template <typename F>
        void start(F f) {
            f(error_code{});
        }

        tcp::socket socket_;
        asio::io_context* io_ctx_;
    };

    #ifdef CROW_ENABLE_SSL
    struct SSLAdaptor {
        using context = asio::ssl::context;
        using ssl_socket_t = asio::ssl::stream<tcp::socket>;

        SSLAdaptor(asio::io_context& io_context, context* ctx) :
            ssl_socket_(std::make_unique<ssl_socket_t>(io_context, *ctx)) {}

        ssl_socket_t& socket() { return *ssl_socket_; }
        tcp::socket& raw_socket() { return ssl_socket_->next_layer(); }
        tcp::endpoint remote_endpoint() { return raw_socket().remote_endpoint(); }
        bool is_open() { return ssl_socket_ && raw_socket().is_open(); }

        error_code close() {
            error_code ec;
            if (!is_open()) {
                ec = boost::asio::error::not_connected;
            } else {
                raw_socket().close(ec);
            }
            return ec;
        }

        error_code shutdown_readwrite() {
            error_code ec;
            if (!is_open()) {
                ec = boost::asio::error::not_connected;
            } else {
                raw_socket().shutdown(boost::asio::socket_base::shutdown_both, ec);
            }
            return ec;
        }

        error_code shutdown_write() {
            error_code ec;
            if (!is_open()) {
                ec = boost::asio::error::not_connected;
            } else {
                raw_socket().shutdown(boost::asio::socket_base::shutdown_send, ec);
            }
            return ec;
        }

        error_code shutdown_read() {
            error_code ec;
            if (!is_open()) {
                ec = boost::asio::error::not_connected;
            } else {
                raw_socket().shutdown(boost::asio::socket_base::shutdown_receive, ec);
            }
            return ec;
        }

        asio::io_context& get_io_context() {
            return static_cast<asio::io_context&>(ssl_socket_->get_executor().context());
        }

        template <typename F>
        void start(F f) {
            ssl_socket_->async_handshake(asio::ssl::stream_base::server,
                                         std::move(f));
        }

        std::unique_ptr<ssl_socket_t> ssl_socket_;
    };
    #endif
}  // namespace crow
