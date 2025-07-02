#pragma once

#include <boost/asio.hpp>
#ifdef CROW_ENABLE_SSL
#include <boost/asio/ssl.hpp>
#endif

namespace asio = boost::asio;
using error_code = boost::system::error_code;

namespace crow {
namespace asio = boost::asio;
using error_code = boost::system::error_code;
#ifdef CROW_ENABLE_SSL
namespace ssl = boost::asio::ssl;
#endif
} // namespace crow
