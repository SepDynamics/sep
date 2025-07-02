#pragma once

#include <boost/asio.hpp>
#include <boost/asio/version.hpp>
#ifdef CROW_ENABLE_SSL
#include <boost/asio/ssl.hpp>
#endif
#include "settings.h"
#include "asio_isolation.h"
#include <system_error>
#include "logging.h"

// Fix for the conditional expression to avoid the operator '&&' error
#ifdef CROW_USE_BOOST
#if defined(BOOST_VERSION) && BOOST_VERSION >= 107000
#define GET_IO_CONTEXT(s) ((asio::io_context&)(s).get_executor().context())
#else
#define GET_IO_CONTEXT(s) ((s).get_io_service())
#endif
#else
#if defined(ASIO_VERSION) && ASIO_VERSION >= 101300
#define GET_IO_CONTEXT(s) ((asio::io_context&)(s).get_executor().context())
#else
#define GET_IO_CONTEXT(s) ((s).get_io_service())
#endif
#endif

namespace crow {
#ifdef CROW_USE_BOOST
namespace asio = boost::asio;
#else
// Use standalone asio
namespace asio = ::asio;
#endif
using error_code = std::error_code;
using tcp = asio::ip::tcp;

/// A wrapper for the asio::ip::tcp::socket and asio::ssl::stream
struct SocketAdaptor {
  using context = void;
  SocketAdaptor(asio::io_context& io_context, context*) : socket_(io_context.get_executor()) {}

  asio::io_context& get_io_context() { return GET_IO_CONTEXT(socket_); }

  /// Get the TCP socket handling data trasfers, regardless of what layer is handling transfers on
  /// top of the socket.
  tcp::socket& raw_socket() { return socket_; }

  /// Get the object handling data transfers, this can be either a TCP socket or an SSL stream (if
  /// SSL is enabled).
  tcp::socket& socket() { return socket_; }

  tcp::endpoint remote_endpoint() { return socket_.remote_endpoint(); }

  bool is_open() { return socket_.is_open(); }

  error_code close() {
    std::error_code ec;
    socket_.close(ec);
    if (ec) {
      CROW_LOG_ERROR << "Socket close error: " << ec.message();
    }
    return error_code(ec);
  }

  error_code shutdown_readwrite() {
    std::error_code ec;
    std::error_code shutdown_ec;
#ifdef CROW_USE_BOOST
    shutdown_ec = socket_.shutdown(asio::socket_base::shutdown_both, ec);
    (void)shutdown_ec;
#else
    shutdown_ec = socket_.shutdown(asio::socket_base::shutdown_both, ec);
    (void)shutdown_ec;
#endif
    if (ec) {
      CROW_LOG_ERROR << "Socket shutdown error: " << ec.message();
    }
    return error_code(ec);
  }

  error_code shutdown_write() {
    std::error_code ec;
    std::error_code shutdown_ec;
#ifdef CROW_USE_BOOST
    shutdown_ec = socket_.shutdown(asio::socket_base::shutdown_send, ec);
    (void)shutdown_ec;
#else
    shutdown_ec = socket_.shutdown(asio::socket_base::shutdown_send, ec);
    (void)shutdown_ec;
#endif
    if (ec) {
      CROW_LOG_ERROR << "Socket shutdown error: " << ec.message();
    }
    return error_code(ec);
  }

  error_code shutdown_read() {
    std::error_code ec;
    std::error_code shutdown_ec;
#ifdef CROW_USE_BOOST
    shutdown_ec = socket_.shutdown(asio::socket_base::shutdown_receive, ec);
    (void)shutdown_ec;
#else
    shutdown_ec = socket_.shutdown(asio::socket_base::shutdown_receive, ec);
    (void)shutdown_ec;
#endif
    if (ec) {
      CROW_LOG_ERROR << "Socket shutdown error: " << ec.message();
    }
    return error_code(ec);
  }

  template <typename F>
  void start(F f) {
    f(error_code());
  }

  tcp::socket socket_;
};

#ifdef CROW_ENABLE_SSL
struct SSLAdaptor {
  using context = asio::ssl::context;
  using ssl_socket_t = asio::ssl::stream<tcp::socket>;
  SSLAdaptor(asio::io_context& io_context, context* ctx)
      : ssl_socket_(new ssl_socket_t(io_context, *ctx)) {}

  asio::ssl::stream<tcp::socket>& socket() { return *ssl_socket_; }

  tcp::socket::lowest_layer_type& raw_socket() { return ssl_socket_->lowest_layer(); }

  tcp::endpoint remote_endpoint() { return raw_socket().remote_endpoint(); }

  bool is_open() { return ssl_socket_ ? raw_socket().is_open() : false; }

  error_code close() {
    error_code ec{};
    if (is_open()) {
      (void)raw_socket().close(ec);
      if (ec) {
        CROW_LOG_ERROR << "Socket close error: " << ec.message();
      }
    }
    return ec;
  }

  error_code shutdown_readwrite() {
    error_code ec{};
    if (is_open()) {
#ifdef CROW_USE_BOOST
      (void)raw_socket().shutdown(asio::socket_base::shutdown_both, ec);
#else
      (void)raw_socket().shutdown(asio::socket_base::shutdown_both, ec);
#endif
      if (ec) {
        CROW_LOG_ERROR << "Socket shutdown error: " << ec.message();
      }
    }
    return ec;
  }

  error_code shutdown_write() {
    error_code ec{};
    if (is_open()) {
#ifdef CROW_USE_BOOST
      (void)raw_socket().shutdown(asio::socket_base::shutdown_send, ec);
#else
      (void)raw_socket().shutdown(asio::socket_base::shutdown_send, ec);
#endif
      if (ec) {
        CROW_LOG_ERROR << "Socket shutdown error: " << ec.message();
      }
    }
    return ec;
  }

  error_code shutdown_read() {
    error_code ec{};
    if (is_open()) {
#ifdef CROW_USE_BOOST
      (void)raw_socket().shutdown(asio::socket_base::shutdown_receive, ec);
#else
      (void)raw_socket().shutdown(asio::socket_base::shutdown_receive, ec);
#endif
      if (ec) {
        CROW_LOG_ERROR << "Socket shutdown error: " << ec.message();
      }
    }
    return ec;
  }

  asio::io_context& get_io_context() { return GET_IO_CONTEXT(raw_socket()); }

  template <typename F>
  void start(F f) {
    ssl_socket_->async_handshake(asio::ssl::stream_base::server,
                                 [f](const error_code& ec) { f(ec); });
  }

  std::unique_ptr<asio::ssl::stream<tcp::socket>> ssl_socket_;
};
#endif
}  // namespace crow
