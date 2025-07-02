#pragma once
#include "../../include/compat/shim.h"
#include "compat/type_id_compat.h"

// This header file is used to isolate ASIO-related code from CUDA compilation
// It provides stub implementations for ASIO-related functionality that can be
// safely included in CUDA files without causing template instantiation errors

// Check for CUDA compilation - either via __CUDACC__ or our custom SEP_CUDA_COMPILATION flag
#if defined(__CUDACC__) || defined(SEP_CUDA_COMPILATION)
// When compiling with CUDA, provide stub implementations

namespace asio {

// Stub implementations for ASIO classes and functions
class io_context
{
public:
    io_context() {}
    io_context(int) {}

    void run() {}
    void stop() {}
    bool stopped() const
    {
        return true;
    }

    template<typename Allocator = sep::shim::allocator<void>, int Index = 0>
    class basic_executor_type
    {
    public:
        // Stub implementation
        template<typename T>
        void connect([[maybe_unused]] T&)
        {}
        template<typename T>
        void on_work_started([[maybe_unused]] T&)
        {}
        template<typename T>
        void on_work_finished([[maybe_unused]] T&)
        {}
    };

    // Add executor_type typedef
    typedef basic_executor_type<> executor_type;

    // Add get_executor method
    executor_type get_executor()
    {
        return executor_type();
    }
};

// Define any_io_executor
class any_io_executor
{
public:
    any_io_executor() {}

    template<typename T>
    any_io_executor(T&&)
    {}
};

// Add executor concept support
template<typename T>
struct is_executor_of
{
    static constexpr bool value = false;
};

// Add execution context concept
class execution_context
{
public:
    execution_context() {}

    template<typename T>
    T& query(const T&)
    {
        static T t;
        return t;
    }
};

// Add service concept
template<typename T>
class execution_context_service_base
{
public:
    execution_context_service_base([[maybe_unused]] execution_context& ctx) {}
};

template<typename T>
class service : public execution_context_service_base<T>
{
public:
    service([[maybe_unused]] execution_context& ctx) : execution_context_service_base<T>(ctx) {}
};

// Add use_service function
template<typename Service>
Service& use_service(execution_context& context)
{
    static Service service(context);
    return service;
}

// Add traits namespace to handle static_query.hpp errors
namespace traits {
// Stub implementation for static_query
template<typename T, typename Property, typename = void>
struct static_query
{
    // Stub implementation
    static constexpr bool is_valid    = false;
    static constexpr bool is_noexcept = false;
    typedef void          result_type;

    // Add static query method to prevent instantiation errors
    template<typename U = void>
    static constexpr auto static_query_v()
    {
        return 0;
    }
};

// Stub implementation for query_static_constexpr_member
template<typename T, typename Property, typename = void>
struct query_static_constexpr_member
{
    static constexpr bool is_valid    = false;
    static constexpr bool is_noexcept = false;
    typedef void          result_type;

    // Add static query method
    template<typename U = void>
    static constexpr auto static_query_v()
    {
        return 0;
    }
};

// Stub implementation for query_member
template<typename T, typename Property, typename = void>
struct query_member
{
    static constexpr bool is_valid    = false;
    static constexpr bool is_noexcept = false;
    typedef void          result_type;

    // Add query method
    template<typename U>
    static constexpr auto query(U&, const Property&)
    {
        return 0;
    }
};

// Stub implementation for query_free
template<typename T, typename Property, typename = void>
struct query_free
{
    static constexpr bool is_valid    = false;
    static constexpr bool is_noexcept = false;
    typedef void          result_type;

    // Add query method
    template<typename U>
    static constexpr auto query(U&, const Property&)
    {
        return 0;
    }
};

// Add additional traits to handle template instantiation errors
template<typename T, typename Executor, typename = void>
struct execute_member
{
    static constexpr bool is_valid    = false;
    static constexpr bool is_noexcept = false;

    // Add execute method
    template<typename U, typename E>
    static constexpr auto execute(U&, E&&)
    {
        return 0;
    }
};

template<typename T, typename CompletionHandler, typename = void>
struct connect_member
{
    static constexpr bool is_valid    = false;
    static constexpr bool is_noexcept = false;

    // Add connect method
    template<typename U, typename C>
    static constexpr auto connect(U&, C&&)
    {
        return 0;
    }
};
}  // namespace traits

namespace execution {
// Add execution property base class
template<int I>
struct property_base
{};

// Add execution property categories
struct blocking_t : property_base<0>
{};
struct relationship_t : property_base<1>
{};
struct outstanding_work_t : property_base<2>
{};
struct allocator_t : property_base<3>
{};

// Add execution property values
constexpr blocking_t         blocking{};
constexpr relationship_t     relationship{};
constexpr outstanding_work_t outstanding_work{};

namespace detail {
namespace blocking {
template<int Index>
struct never_t : property_base<Index>
{};

template<int Index>
struct possibly_t : property_base<Index>
{};

template<int Index>
struct always_t : property_base<Index>
{};

// Add static query support
template<typename T>
struct static_query_base
{
    static constexpr bool is_valid    = false;
    static constexpr bool is_noexcept = false;
    typedef int           result_type;
    static constexpr int  value()
    {
        return 0;
    }
};
}  // namespace blocking

namespace outstanding_work {
template<int Index>
struct tracked_t : property_base<Index>
{};

template<int Index>
struct untracked_t : property_base<Index>
{};

// Add static query support
template<typename T>
struct static_query_base
{
    static constexpr bool is_valid    = false;
    static constexpr bool is_noexcept = false;
    typedef int           result_type;
    static constexpr int  value()
    {
        return 0;
    }
};
}  // namespace outstanding_work

namespace relationship {
template<int Index>
struct fork_t : property_base<Index>
{};

template<int Index>
struct continuation_t : property_base<Index>
{};

// Add static query support
template<typename T>
struct static_query_base
{
    static constexpr bool is_valid    = false;
    static constexpr bool is_noexcept = false;
    typedef int           result_type;
    static constexpr int  value()
    {
        return 0;
    }
};
}  // namespace relationship
}  // namespace detail

template<typename T>
struct prefer_only : property_base<100>
{};

template<typename T>
struct context_as_t : property_base<101>
{};

// Add any_executor concept
template<typename... SupportableProperties>
class any_executor
{
public:
    any_executor() {}

    template<typename T>
    any_executor(T&&)
    {}

    // Add required methods to prevent instantiation errors
    template<typename Property>
    auto query(const Property&) const
    {
        return 0;
    }
};

// Add query function
template<typename T, typename Property>
auto query(const T&, const Property&)
{
    return 0;
}
}  // namespace execution

// Add additional stubs for template-related errors
template<typename T>
struct is_executor
{
    static constexpr bool value = false;
};

template<typename T, typename Property>
struct can_query
{
    static constexpr bool value = false;
};

template<typename T, typename Property>
struct can_require
{
    static constexpr bool value = false;
};

template<typename T, typename Property>
struct can_prefer
{
    static constexpr bool value = false;
};

// Add additional executor trait stubs
template<typename T, typename U>
struct is_applicable_property
{
    static constexpr bool value = false;
};

// Add error_code and system_error stubs
class error_category
{
public:
    error_category() {}
    virtual ~error_category() {}
    virtual const char* name() const
    {
        return "";
    }
    virtual sep::shim::string message(int) const
    {
        return "";
    }
};

inline const error_category& system_category()
{
    static error_category cat;
    return cat;
}

inline const error_category& generic_category()
{
    static error_category cat;
    return cat;
}

class error_code
{
public:
    error_code() {}
    error_code(int, const error_category&) {}
    operator bool() const
    {
        return false;
    }
    int value() const
    {
        return 0;
    }
    const error_category& category() const
    {
        return system_category();
    }
};

class system_error
{
public:
    system_error(const error_code&) {}
    system_error(int, const error_category&) {}
    const char* what() const
    {
        return "";
    }
    const error_code& code() const
    {
        static error_code ec;
        return ec;
    }
};

// Add buffer concept stubs
template<typename T>
struct is_const_buffer_sequence
{
    static constexpr bool value = false;
};

template<typename T>
struct is_mutable_buffer_sequence
{
    static constexpr bool value = false;
};

class const_buffer
{
public:
    const_buffer() {}
    const_buffer(const void*, std::size_t) {}
    const void* data() const
    {
        return nullptr;
    }
    std::size_t size() const
    {
        return 0;
    }
};

class mutable_buffer
{
public:
    mutable_buffer() {}
    mutable_buffer(void*, std::size_t) {}
    void* data() const
    {
        return nullptr;
    }
    std::size_t size() const
    {
        return 0;
    }
};

// Add buffer function
inline const_buffer buffer(const void* data, std::size_t size)
{
    return const_buffer(data, size);
}

inline mutable_buffer buffer(void* data, std::size_t size)
{
    return mutable_buffer(data, size);
}

// Add async I/O functions
template<typename AsyncReadStream, typename MutableBufferSequence, typename CompletionToken>
int async_read([[maybe_unused]] AsyncReadStream&,
               [[maybe_unused]] const MutableBufferSequence&,
               [[maybe_unused]] CompletionToken&&)
{
    return 0;
}

template<typename AsyncWriteStream, typename ConstBufferSequence, typename CompletionToken>
int async_write([[maybe_unused]] AsyncWriteStream&,
                [[maybe_unused]] const ConstBufferSequence&,
                [[maybe_unused]] CompletionToken&&)
{
    return 0;
}

template<typename SyncReadStream, typename MutableBufferSequence>
std::size_t read([[maybe_unused]] SyncReadStream&, [[maybe_unused]] const MutableBufferSequence&)
{
    return 0;
}

template<typename SyncWriteStream, typename ConstBufferSequence>
std::size_t write([[maybe_unused]] SyncWriteStream&, [[maybe_unused]] const ConstBufferSequence&)
{
    return 0;
}

// Add handler work tracking
template<typename Handler, typename Executor>
class handler_work
{
public:
    handler_work(Handler&, const Executor&) {}
    void start() {}
    void complete() {}
};

// Add deadline timer service
template<typename Time, typename TimeTraits>
class deadline_timer_service : public execution_context_service_base<deadline_timer_service<Time, TimeTraits>>
{
public:
    deadline_timer_service([[maybe_unused]] execution_context& ctx)
        : execution_context_service_base<deadline_timer_service<Time, TimeTraits>>(ctx)
    {}
};

// Add waitable timer
template<typename Clock, typename WaitTraits = void>
class basic_waitable_timer
{
public:
    typedef any_io_executor executor_type;

    basic_waitable_timer([[maybe_unused]] io_context& ctx) {}
    basic_waitable_timer([[maybe_unused]] const executor_type& ex) {}

    executor_type get_executor()
    {
        return executor_type();
    }

    template<typename CompletionToken>
    auto async_wait([[maybe_unused]] CompletionToken&& token)
    {
        return 0;
    }
};

// Add post and dispatch functions
template<typename Executor, typename CompletionHandler>
void post([[maybe_unused]] const Executor&, [[maybe_unused]] CompletionHandler&&)
{}

template<typename Executor, typename CompletionHandler>
void dispatch([[maybe_unused]] const Executor&, [[maybe_unused]] CompletionHandler&&)
{}

// Add signal_set
class signal_set
{
public:
    signal_set([[maybe_unused]] io_context& ctx) {}

    template<typename CompletionHandler>
    void async_wait([[maybe_unused]] CompletionHandler&&)
    {}
};

// Add ip namespace for networking support
namespace ip {
// Add tcp protocol
class tcp {
public:
    class socket {
    public:
        socket([[maybe_unused]] io_context& ctx) {}
        void close() {}
        void close([[maybe_unused]] error_code& ec) {}
        void shutdown([[maybe_unused]] int type, [[maybe_unused]] error_code& ec) {}
        bool is_open() const { return false; }
        
        class endpoint {
        public:
            endpoint() {}
        };
        
        endpoint remote_endpoint() const { return endpoint(); }
        endpoint remote_endpoint([[maybe_unused]] error_code& ec) const { return endpoint(); }
    };
    
    using endpoint = socket::endpoint;
};

// Add socket_base for shutdown types
class socket_base {
public:
    static constexpr int shutdown_both = 0;
    static constexpr int shutdown_send = 1;
    static constexpr int shutdown_receive = 2;
};
} // namespace ip

// Add socket_base to main asio namespace as well
class socket_base {
public:
    static constexpr int shutdown_both = 0;
    static constexpr int shutdown_send = 1;
    static constexpr int shutdown_receive = 2;
};
}  // namespace asio

#else
// When not compiling with CUDA, we need to include ASIO but prevent conflicts
// We'll define a macro to ensure we only include it once
#    ifndef SEP_ASIO_STANDALONE_DEFINED
#    define SEP_ASIO_STANDALONE_DEFINED
#    ifndef ASIO_STANDALONE
#    define ASIO_STANDALONE
#    endif
#    endif

// Only include ASIO once to prevent redefinition errors
#    ifndef SEP_ASIO_INCLUDED
#    define SEP_ASIO_INCLUDED

// Choose between Boost ASIO and standalone ASIO
#        include <boost/asio.hpp>
namespace asio = boost::asio;

#    endif // SEP_ASIO_INCLUDED
#endif
