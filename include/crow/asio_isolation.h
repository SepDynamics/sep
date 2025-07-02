#pragma once

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

// Historical namespace used by the project when ASIO functionality was stubbed.
// It now aliases Boost.ASIO directly so that existing code continues to compile
// while using the real implementation.
namespace crow_asio_stub = boost::asio;

// Some tests include this header expecting an `asio` namespace.
namespace asio = boost::asio;

namespace crow {
namespace asio_stub = boost::asio;
using error_code = boost::system::error_code;
} // namespace crow
