#pragma once

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

#include "compat/shim.h"
#include "crow/logging.h"
#include "crow/asio_isolation.h"

namespace crow {
using tcp = boost::asio::ip::tcp;

struct SocketAdaptor {
    using context = void;
    SocketAdaptor(boost::asio::io_context& io_context, context*)
        : io_context_(io_context), socket_(io_context) {}

    boost::asio::io_context& get_io_context() { return io_context_; }
    tcp::socket& raw_socket() { return socket_; }
    tcp::socket& socket() { return socket_; }
    tcp::endpoint remote_endpoint() {
        boost::system::error_code ec;
        return socket_.remote_endpoint(ec);
    }
    bool is_open() { return socket_.is_open(); }

    error_code close() {
        error_code ec;
        socket_.close(ec);
        if (!socket_.is_open() && !ec)
            ec = boost::asio::error::not_connected;
        return ec;
    }

    error_code shutdown_readwrite() {
        error_code ec;
        socket_.shutdown(tcp::socket::shutdown_both, ec);
        if (!socket_.is_open() && !ec)
            ec = boost::asio::error::not_connected;
        return ec;
    }

    error_code shutdown_write() {
        error_code ec;
        socket_.shutdown(tcp::socket::shutdown_send, ec);
        if (!socket_.is_open() && !ec)
            ec = boost::asio::error::not_connected;
        return ec;
    }

    error_code shutdown_read() {
        error_code ec;
        socket_.shutdown(tcp::socket::shutdown_receive, ec);
        if (!socket_.is_open() && !ec)
            ec = boost::asio::error::not_connected;
        return ec;
    }

    template <typename F>
    void start(F f) { f(error_code()); }

    boost::asio::io_context& io_context_;
    tcp::socket              socket_;
};

#ifdef CROW_ENABLE_SSL
struct SSLAdaptor {
    using context = boost::asio::ssl::context;
    using ssl_socket_t = boost::asio::ssl::stream<tcp::socket>;

    SSLAdaptor(boost::asio::io_context& io_context, context* ctx)
        : io_context_(io_context),
          ssl_socket_(std::make_unique<ssl_socket_t>(io_context, *ctx)) {}

    ssl_socket_t& socket() { return *ssl_socket_; }
    tcp::socket& raw_socket() { return ssl_socket_->next_layer(); }
    tcp::endpoint remote_endpoint() {
        boost::system::error_code ec;
        return raw_socket().remote_endpoint(ec);
    }
    bool is_open() { return ssl_socket_ ? raw_socket().is_open() : false; }

    error_code close() {
        error_code ec;
        if (is_open())
            raw_socket().close(ec);
        if (!is_open() && !ec)
            ec = boost::asio::error::not_connected;
        return ec;
    }

    error_code shutdown_readwrite() {
        error_code ec;
        if (is_open())
            raw_socket().shutdown(tcp::socket::shutdown_both, ec);
        if (!is_open() && !ec)
            ec = boost::asio::error::not_connected;
        return ec;
    }

    error_code shutdown_write() {
        error_code ec;
        if (is_open())
            raw_socket().shutdown(tcp::socket::shutdown_send, ec);
        if (!is_open() && !ec)
            ec = boost::asio::error::not_connected;
        return ec;
    }

    error_code shutdown_read() {
        error_code ec;
        if (is_open())
            raw_socket().shutdown(tcp::socket::shutdown_receive, ec);
        if (!is_open() && !ec)
            ec = boost::asio::error::not_connected;
        return ec;
    }

    boost::asio::io_context& get_io_context() { return io_context_; }

    template <typename F>
    void start(F f) {
        ssl_socket_->async_handshake(boost::asio::ssl::stream_base::server,
                                     std::move(f));
    }

    boost::asio::io_context&        io_context_;
    std::unique_ptr<ssl_socket_t>   ssl_socket_;
};
#endif

} // namespace crow
