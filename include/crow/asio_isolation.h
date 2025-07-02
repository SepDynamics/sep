#pragma once

// Provide Boost.ASIO types without pulling in heavy headers when not needed.
// This header previously defined stub implementations. It now aliases the
// required classes directly to Boost.ASIO so networking code uses the real
// library.

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

// Alias the Boost.ASIO namespace so existing code using `asio_stub` or `asio`
// continues to compile.
namespace crow_asio_stub = boost::asio;
namespace asio = boost::asio;

namespace crow {
// Alias the stub namespace inside crow as before
namespace asio_stub = ::crow_asio_stub;
using error_code = boost::system::error_code;
} // namespace crow
