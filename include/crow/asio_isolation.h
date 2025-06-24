#pragma once

// This is a fixed version of the asio_isolation.h file from the Crow framework
// It provides stub implementations for ASIO functionality

// Include our own headers
#include "compat/shim.h"

// This header file is used to isolate ASIO-related code from CUDA compilation
// It provides stub implementations for ASIO-related functionality that can be
// safely included in CUDA files without causing template instantiation errors

// IMPORTANT: We're using a different namespace to avoid conflicts with the system's asio
namespace crow_asio_stub {
    // Forward declare the error_code class from crow namespace
    class error_code {
    public:
        error_code() : value_(0) {}
        error_code(int val) : value_(val) {}
        
        // Allow conversion to bool for if(ec) checks
        operator bool() const { return value_ != 0; }
        
        // Provide a message method for logging
        sep::shim::string message() const { return "Error code"; }
        
    private:
        int value_;
    };

    // Stub implementations for ASIO classes and functions
    class io_context {
    public:
        io_context() {}
        io_context(int) {}

        void run() {}
        void stop() {}
        bool stopped() const {
            return true;
        }

        class executor_type {
        public:
            executor_type() {}
            
            class context {
            public:
                context() {}
            };
            
            context context() { return context(); }
        };
        
        executor_type get_executor() { return executor_type(); }
    };

    // Define any_io_executor
    class any_io_executor {
    public:
        any_io_executor() {}

        template<typename T>
        any_io_executor(T&&) {}
    };

    // Add executor concept support
    template<typename T>
    struct is_executor_of {
        static constexpr bool value = false;
    };

    // Add execution context concept
    class execution_context {
    public:
        execution_context() {}

        template<typename T>
        T& query(const T&) {
            static T t;
            return t;
        }
    };

    // Add service concept
    template<typename T>
    class execution_context_service_base {
    public:
        execution_context_service_base(execution_context& ctx) {}
    };

    template<typename T>
    class service : public execution_context_service_base<T> {
    public:
        service(execution_context& ctx) : execution_context_service_base<T>(ctx) {}
    };

    // Add use_service function
    template<typename Service>
    Service& use_service(execution_context& context) {
        static Service service(context);
        return service;
    }

    namespace socket_base {
        enum shutdown_type {
            shutdown_receive,
            shutdown_send,
            shutdown_both
        };
    }

    namespace ip {
        class tcp {
        public:
            class endpoint {
            public:
                endpoint() {}
                endpoint(const sep::shim::string&, int) {}
            };
            
            class socket {
            public:
                // Default constructor
                socket() {}
                
                // Constructor with io_context
                socket(io_context& io) {}
                
                // Constructor with executor
                socket(const io_context::executor_type& ex) {}
                
                bool is_open() const { return false; }
                
                // Close method that takes our error_code
                void close(error_code& ec) { ec = error_code(0); }
                
                // Shutdown method that takes our error_code
                void shutdown(socket_base::shutdown_type type, error_code& ec) { 
                    ec = error_code(0); 
                }
                
                endpoint remote_endpoint() { return endpoint(); }
                
                typedef socket lowest_layer_type;
                lowest_layer_type& lowest_layer() { return *this; }
                
                io_context& get_io_service() { 
                    static io_context io;
                    return io;
                }
                
                io_context::executor_type get_executor() { return io_context::executor_type(); }
            };
        };
    }
    
    #ifdef CROW_ENABLE_SSL
    namespace ssl {
        class context {
        public:
            context(int method) {}
        };
        
        template<typename Socket>
        class stream {
        public:
            stream(io_context& io, context& ctx) {}
            
            typedef typename Socket::lowest_layer_type lowest_layer_type;
            
            lowest_layer_type& lowest_layer() { 
                static typename Socket::lowest_layer_type layer;
                return layer;
            }
            
            template<typename HandshakeHandler>
            void async_handshake(int type, HandshakeHandler&& handler) {
                handler(error_code());
            }
        };
        
        enum {
            server = 0
        };
    }
    #endif
}

// Use our stub namespace in the crow namespace
namespace crow {
    namespace asio_stub = crow_asio_stub;
    
    // Use the error_code from our stub namespace
    using error_code = crow_asio_stub::error_code;
}