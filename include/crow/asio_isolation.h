#pragma once

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/system/error_code.hpp>

// Alias Boost.ASIO types so CUDA compilation can include this header
namespace crow_asio_stub = boost::asio;
namespace asio = boost::asio;

namespace crow {
    namespace asio_stub = boost::asio;
    using error_code = boost::system::error_code;
} // namespace crow
