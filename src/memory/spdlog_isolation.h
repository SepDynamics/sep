#pragma once

#include <mutex>
#include <sstream>
#include <string>
#include <vector>

#include "engine/internal/cuda.h"
#include "engine/internal/macros.h"

#define SEP_SPDLOG_AVAILABLE 1

namespace sep {
namespace spdlog {

// CUDA-compatible formatter base class
class formatter
{
public:
    virtual ~formatter() = default;
};

// Default pattern formatter implementation
class pattern_formatter : public formatter
{
public:
    SEP_HOST SEP_DEVICE void format(const std::string& msg, std::ostringstream& dest)
    {
        dest << msg.c_str();
    }

    SEP_HOST SEP_DEVICE std::string pattern() const { return "%v"; }
};

}  // namespace spdlog
}  // namespace sep

// This header file is used to isolate spdlog-related code from CUDA compilation
// It provides stub implementations for spdlog functionality that can be
// safely included in CUDA files without causing template instantiation errors

// Fallback implementation when CUDA is used or spdlog headers are unavailable
#if defined(SEP_SPDLOG_FALLBACK)
// When compiling with CUDA, provide stub implementations

// Include our isolation headers
#include "engine/internal/standard_includes.h"
#if !SEP_CUDA_AVAILABLE
// Host builds can include standard library headers directly
#    include <atomic>
#    include <memory>
#    include <ratio>
#else
// CUDA device builds lack full standard library support; use forward declarations
#endif

// Forward declarations or aliases for standard library components used by spdlog
namespace sep::shim {
#if SEP_CUDA_AVAILABLE
// Ratio template for chrono
template<intmax_t Num, intmax_t Denom = 1>
struct ratio
{
    static constexpr intmax_t num = Num;
    static constexpr intmax_t den = Denom;
};

template<typename T>
class atomic;

template<class T, class Deleter = std::default_delete<T>>
class unique_ptr;

template<typename T>
class shared_ptr;

template<typename T>
class weak_ptr;

template<typename T>
class function;

template<typename T, typename U>
class pair;

class thread;

template<typename Mutex>
class unique_lock;

enum class memory_order
{
    relaxed,
    consume,
    acquire,
    release,
    acq_rel,
    seq_cst
};

constexpr memory_order memory_order_relaxed = memory_order::relaxed;
constexpr memory_order memory_order_consume = memory_order::consume;
constexpr memory_order memory_order_acquire = memory_order::acquire;
constexpr memory_order memory_order_release = memory_order::release;
constexpr memory_order memory_order_acq_rel = memory_order::acq_rel;
constexpr memory_order memory_order_seq_cst = memory_order::seq_cst;

template<typename T, typename... Args>
shared_ptr<T> make_shared(Args&&...)
{
    return shared_ptr<T>();
}

template<typename T, typename... Args>
unique_ptr<T> make_unique(Args&&...)
{
    return unique_ptr<T>();
}
#else
using std::ratio;
using std::atomic;
using std::unique_ptr;
using std::shared_ptr;
using std::weak_ptr;
using std::function;
using std::pair;
using std::thread;
using std::unique_lock;
using std::memory_order;
using std::memory_order_relaxed;
using std::memory_order_consume;
using std::memory_order_acquire;
using std::memory_order_release;
using std::memory_order_acq_rel;
using std::memory_order_seq_cst;
using std::make_shared;
using std::make_unique;
#endif

// Chrono namespace for time-related functionality
namespace chrono {
class duration_base
{};

template<typename Rep, typename Period = ratio<1>>
class duration : public duration_base
{
public:
    duration() {}
    explicit duration(const Rep&) {}
    
    template<typename Rep2>
    duration(const Rep2&)
    {}

    template<typename Rep2, typename Period2>
    duration(const duration<Rep2, Period2>&)
    {}
};

typedef duration<int64_t, ratio<1>>             seconds;
typedef duration<int64_t, ratio<1, 1000>>       milliseconds;
typedef duration<int64_t, ratio<1, 1000000>>    microseconds;
typedef duration<int64_t, ratio<1, 1000000000>> nanoseconds;
}  // namespace chrono
}  // namespace sep::shim

// Forward declarations for fmt library
namespace fmt {
template<typename... T>
class format_string
{
public:
    format_string() {}
};

template<typename T>
class basic_string_view
{
public:
    basic_string_view() {}
    basic_string_view(const char*) {}
};

template<typename T>
class basic_memory_buffer
{
public:
    basic_memory_buffer() {}
};

template<typename T>
class basic_runtime
{
public:
    basic_runtime() {}
};

using string_view   = basic_string_view<char>;
using memory_buffer = basic_memory_buffer<char>;
using runtime       = basic_runtime<char>;
}  // namespace fmt

// Stub implementations for spdlog
namespace spdlog {
// Forward declarations
class logger;

// Enums
enum class level
{
    trace,
    debug,
    info,
    warn,
    err,
    critical,
    off
};

// Stub for logger class
class logger
{
public:
    logger() {}
    logger(const sep::string&) {}

    template<typename... Args>
    void log(level, const fmt::format_string<Args...>&, Args&&...)
    {}

    template<typename... Args>
    void trace(const fmt::format_string<Args...>&, Args&&...)
    {}

    template<typename... Args>
    void debug(const fmt::format_string<Args...>&, Args&&...)
    {}

    template<typename... Args>
    void info(const fmt::format_string<Args...>&, Args&&...)
    {}

    template<typename... Args>
    void warn(const fmt::format_string<Args...>&, Args&&...)
    {}

    template<typename... Args>
    void error(const fmt::format_string<Args...>&, Args&&...)
    {}

    template<typename... Args>
    void critical(const fmt::format_string<Args...>&, Args&&...)
    {}
};

// Stub for registry
namespace details {
class registry
{
public:
    inline static registry& instance()
    {
        static registry instance;
        static bool     initialized = false;
        if (!initialized)
        {
            initialized = true;
            instance.set_level(level::info);
        }
        return instance;
    }

    sep::shared_ptr<logger> get(const sep::string&)
    {
        try
        {
            return sep::make_shared<logger>();
        }
        catch (const sep::exception& e)
        {
            (void)fprintf(stderr, "Failed to create logger: %s\n", e.what());
            return nullptr;
        }
    }

    void register_logger(sep::shared_ptr<logger>) {}

    void set_default_logger(sep::shared_ptr<logger>) {}

    void drop(const sep::string&) {}

    void drop_all() {}

    void shutdown() {}

    void set_automatic_registration(bool) {}

    void set_level(level) {}

    void flush_on(level) {}

    void flush_every(sep::chrono::seconds) {}
};
} // namespace details
} // namespace spdlog

#elif defined(SEP_SPDLOG_AVAILABLE)
#    include <spdlog/spdlog.h>
#    include <spdlog/details/registry.h>
#    include <chrono>

namespace sep {
namespace spdlog {

using logger = ::spdlog::logger;
using level  = ::spdlog::level::level_enum;

namespace details {
class registry
{
public:
    inline static registry& instance()
    {
        static registry inst;
        return inst;
    }

    std::shared_ptr<logger> get(const std::string& name) { return ::spdlog::get(name); }

    void register_logger(std::shared_ptr<logger> lg)
    {
        ::spdlog::register_logger(std::move(lg));
    }

    void set_default_logger(std::shared_ptr<logger> lg)
    {
        ::spdlog::set_default_logger(std::move(lg));
    }

    void drop(const std::string& name) { ::spdlog::drop(name); }

    void drop_all()
    {
        ::spdlog::drop_all();
    }

    void shutdown()
    {
        ::spdlog::shutdown();
    }

    void set_automatic_registration(bool v)
    {
        ::spdlog::details::registry::instance().set_automatic_registration(v);
    }

    void set_level(level lvl)
    {
        ::spdlog::set_level(static_cast<::spdlog::level::level_enum>(lvl));
    }

    void flush_on(level lvl)
    {
        ::spdlog::flush_on(static_cast<::spdlog::level::level_enum>(lvl));
    }

    void flush_every(std::chrono::seconds interval)
    {
        ::spdlog::flush_every(interval);
    }
};
} // namespace details
} // namespace spdlog
} // namespace sep
#endif // SEP_SPDLOG_FALLBACK
