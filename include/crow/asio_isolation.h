#pragma once

// Provide ASIO types without pulling in heavy headers when not needed. This
// project relies on the standalone version of ASIO so we include the standard
// headers and alias the namespace used throughout the code base.

#include <asio.hpp>
#include <asio/ssl.hpp>

// Alias the ASIO namespace so existing code using `asio_stub` or `asio`
// continues to compile regardless of whether Boost or standalone ASIO is used.
namespace crow_asio_stub = ::asio;
namespace asio = ::asio;

namespace crow {
// Alias the stub namespace inside crow as before
namespace asio_stub = ::crow_asio_stub;
using error_code = ::asio::error_code;
} // namespace crow
