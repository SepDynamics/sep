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

// Add the missing asio_stub namespace as an alias to standard asio for compatibility
namespace asio_stub
{
    using namespace ::asio;

    // Add nested namespaces required by the socket adaptor
    namespace ip
    {
        using tcp = ::asio::ip::tcp;
    }

    namespace error
    {
        // Direct access to not_connected value in asio::error namespace
        constexpr auto not_connected = ::asio::error::not_connected;
    }

#ifdef CROW_ENABLE_SSL
    namespace ssl
    {
        using namespace ::asio::ssl;
        using context = ::asio::ssl::context;
    }  // namespace ssl
#endif
}  // namespace asio_stub
