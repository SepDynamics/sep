#pragma once

// IMPORTANT: _GNU_SOURCE is now assumed to be defined by the build system
// (e.g., via CMake's compile definitions like -D_GNU_SOURCE).
// Defining it here would cause a redefinition warning.

// --- C Standard Library Headers (.h versions for global namespace visibility) ---
// These headers make C functions available in the global namespace (::).
// This is often necessary for C++ <c...> wrappers that use 'using ::func_name;' directives.
#include <string.h>   // For C-style string manipulation (memcpy, memset, strlen, strcmp, etc.)
#include <time.h>     // For C-style time functions (time, clock, tm, mktime, localtime, gmtime, timespec_get)
                      // Also includes CLOCK_MONOTONIC when _GNU_SOURCE is defined elsewhere.
#include <unistd.h>   // For POSIX functions (nanosleep, getpid, etc.)
#include <stdlib.h>   // For general utilities (malloc, free, getenv, atoi, etc.)
#include <stdio.h>    // For input/output (snprintf, fprintf, etc.)
#include <stdint.h>   // For fixed-width integer types (uint32_t, uint64_t)
#include <stdbool.h>  // For C-style boolean type (bool)
#include <stddef.h>   // For size_t, ptrdiff_t
#include <math.h>     // For C-style mathematical functions (sin, cos, sqrt, exp, etc.)
#include <setjmp.h>   // For non-local jumps (__jmp_buf)


// --- C++ Standard Library Headers (Preferred C++ style, may depend on .h versions) ---
// These provide C-compatible functions within the std:: namespace.
// Including the .h versions first often helps resolve conflicts or missing declarations.
#include <cstring>    // C++ wrapper for <string.h>
#include <ctime>      // C++ wrapper for <time.h>
#include <cstdlib>    // C++ wrapper for <stdlib.h>
#include <cstdio>     // C++ wrapper for <stdio.h>
#include <cmath>      // C++ wrapper for <math.h>
#include <cstdint>    // C++ wrapper for <stdint.h>
#include <cstddef>    // C++ wrapper for <stddef.h>

// --- Core C++ Language Support Headers ---
// These are fundamental C++ headers.
#include <atomic>              // For std::atomic types
#include <chrono>              // For std::chrono time points, durations, clocks
#include <condition_variable>  // For std::condition_variable
#include <functional>          // For std::function
#include <memory>              // For std::unique_ptr, std::shared_ptr, std::allocator
#include <mutex>               // For std::mutex, std::lock_guard
#include <ostream>             // For std::ostream
#include <queue>               // For std::queue (from audio/pipeline.cpp)
#include <string>              // For std::string
#include <thread>              // For std::thread, std::this_thread
#include <vector>              // For std::vector

// --- Additional common C++ headers you might need, depending on full usage ---
#include <algorithm>  // For std::min, std::max, std::clamp, std::sort, std::find, std::remove_if, std::transform etc.
#include <fstream>    // For std::ifstream, std::ofstream
#include <iostream>   // For std::cerr
#include <limits>     // For std::numeric_limits
#include <map>        // For std::map
#include <optional>   // For std::optional
#include <sstream>    // For std::stringstream
#include <stdexcept>  // For standard exception types (e.g., std::runtime_error, std::invalid_argument)
#include <unordered_map>  // For std::unordered_map
#include <utility>        // For std::move, std::exchange, std::pair

namespace sep {

    enum class SEPResult
    {
        SUCCESS,
        UNKNOWN_ERROR,
        CUDA_ERROR,
        INVALID_ARGUMENT,
        PROCESSING_ERROR,
        OUT_OF_MEMORY,
        NOT_IMPLEMENTED,
        NOT_FOUND
    };

    struct PinState
    {
        std::uint32_t pin_id;
        float value;
        float coherence;
    };

}  // namespace sep
