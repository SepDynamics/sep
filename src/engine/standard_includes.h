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

    template <typename T>
    using vector = std::vector<T>;

    template <typename T>
    using unique_ptr = std::unique_ptr<T>;

    using ifstream = std::ifstream;
    using istringstream = std::istringstream;
    auto& cerr = std::cerr;
    
    // Use std::string directly instead of custom implementation
    using string = std::string;

    // String concatenation - already supported by std::string
    // No need for custom operators

    // Exception classes - use std::exception hierarchy
    using exception = std::exception;
    using runtime_error = std::runtime_error;
    using system_error = std::system_error;

    // Error handling - use std::error_code
    using error_code = std::error_code;
    using error_condition = std::error_condition;
    using error_category = std::error_category;

    inline const error_category& generic_category()
    {
        return std::generic_category();
    }

    inline const error_category& system_category()
    {
        return std::system_category();
    }

    inline error_code make_error_code(std::errc e) { return std::make_error_code(e); }

    inline error_condition make_error_condition(std::errc e) { return std::make_error_condition(e); }

    // String conversions - use std::string functions
    inline int stoi(const string& s) { return std::stoi(s); }
    inline long stol(const string& s) { return std::stol(s); }
    inline long long stoll(const string& s) { return std::stoll(s); }
    inline double stod(const string& s) { return std::stod(s); }
    inline float stof(const string& s) { return std::stof(s); }

    inline string to_string(int val) { return std::to_string(val); }

    // Type traits - use std types
    using true_type = std::true_type;
    using false_type = std::false_type;

    // Math functions - use std math
    using std::exp;
    using std::isfinite;
    using std::isinf;
    using std::isnan;
    using std::lgamma;
    using std::log;
    using std::log2;
    using std::nextafter;
    using std::pow;
    using std::signbit;
    using std::abs;
    using std::sqrt;

    // Memory management
    inline void* aligned_alloc(size_t alignment, size_t size)
    {
        return std::aligned_alloc(alignment, size);
    }

    // Smart pointer helpers
    template<typename T, typename... Args>
    unique_ptr<T> make_unique(Args&&... args) {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    namespace chrono
    {
        using namespace std::chrono;
        using time_point = std::chrono::time_point<std::chrono::high_resolution_clock>;

        inline time_point now() { return std::chrono::high_resolution_clock::now(); }

        template <typename T>
        inline double duration_cast_ms(T duration)
        {
            return std::chrono::duration<double, std::milli>(duration).count();
        }
    }  // namespace chrono

    // Hashing for std::string (which is now std::string)
    struct string_hash
    {
        using is_transparent = void;
        std::size_t operator()(const char* s) const { return std::hash<std::string_view>{}(s); }
        std::size_t operator()(const string& s) const
        {
            return std::hash<string>{}(s);
        }
    };

    // IO helpers
    using stringstream = std::stringstream;

    template<typename T>
    inline std::istream& getline(std::istream& is, T& str, char delim = '\n') {
        return std::getline(is, str, delim);
    }

}  // namespace sep
