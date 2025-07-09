#pragma once

// Prevent ASIO namespace collision by not using the global namespace
#ifndef ASIO_STANDALONE
#define ASIO_STANDALONE
#endif

#include <asio.hpp>

namespace sep {
namespace crow {
namespace asio_detail {
using namespace ::asio;
} // namespace asio_detail
} // namespace crow
} // namespace sep
