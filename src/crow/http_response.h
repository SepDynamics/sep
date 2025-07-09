#pragma once

// Handle ASIO/Crow includes based on RTTI availability
#ifndef CROW_DISABLE_RTTI
// Use real headers in non-CUDA mode
#include "crow/http_response.h"
#else
// Use isolation headers in CUDA mode
#include "crow_isolation.h"
#endif

// Define status codes if not available from the real crow
#ifndef CROW_DISABLE_RTTI
// Use the real status codes
namespace sep
{
    namespace http_status
    {
        using ::crow::status;
    }
}  // namespace sep
#else
// Define our own status codes
namespace crow
{
    enum class status
    {
        UNAUTHORIZED = 401
    };
}
namespace sep
{
    namespace http_status
    {
        using ::crow::status;
    }
}  // namespace sep
#endif
