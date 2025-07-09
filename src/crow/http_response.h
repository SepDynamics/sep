#pragma once

// Include the crow_isolation.h file which defines the status enum
#include "crow/crow_isolation.h"

// Forward declare any additional types to avoid conflicts
namespace crow {
    // Forward declare the response type if not already defined
    #ifndef CROW_RESPONSE_INCLUDED
    struct response;
    #define CROW_RESPONSE_INCLUDED
    #endif
}

namespace sep {
    namespace http_status {
        // Use the status enum from crow_isolation.h
        using crow::status;
    }
}  // namespace sep
