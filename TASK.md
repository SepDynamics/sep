
include/api/crow_request.h:
  51:         // Call the stub's get_header_value

include/compat/cuda_common.h:
  18: // The stub implementation shares the same cudaError_t type defined in
  20: // mode. Using the fully qualified name from the stub namespace caused build

include/compat/cuda_impl.h:
  25:     // Use the namespaced wrapper to work both with CUDA and stub builds
  40: // Async memset is not available in stub builds; provide simple wrapper

include/compat/cuda_runtime.h:
  75: // When CUDA is not available, define stub types and functions

include/compat/cuda_unified_fix.h:
   30: #ifndef CUDA_UNIFIED_FIX_ENABLE_MATH_STUBS
   31: #define CUDA_UNIFIED_FIX_ENABLE_MATH_STUBS 1
  191: // Define stubs for long double math functions that GCC-14 expects but are missing in CUDA
  436: // *** CUDA IMPLEMENTATION STUBS ***

include/compat/cuda.h:
  10: // Include the compatibility header that defines stub types

include/compat/cufft.h:
  16: // When CUDA is not available, define stub types and functions

include/compat/memory.h:
  81: // Stub implementations when CUDA is not available

include/compat/shim.h:
  414:   // Containers - minimal stub implementations

include/crow/app.h:
  4: // It provides stub implementations for the Crow application class

include/crow/asio_isolation.h:
    4: // It provides stub implementations for ASIO functionality
   10: // It provides stub implementations for ASIO-related functionality that can be
   14: namespace crow_asio_stub {
   31:     // Stub implementations for ASIO classes and functions
  190: // Use our stub namespace in the crow namespace
  192:     namespace asio_stub = crow_asio_stub;
  194:     // Use the error_code from our stub namespace
  195:     using error_code = crow_asio_stub::error_code;

include/crow/common.h:
    4: // It provides stub implementations for common functionality
   25:     // Stub implementation for HTTP methods
   99:     // Stub implementation for HTTP status codes
  137:     // Stub implementation for routing_handle_result

include/crow/crow_isolation.h:
    6: // It provides stub implementations for Crow functionality that can be
   44: // Crow namespace with stub implementations
   46:     // Stub implementation for request
   61:     // Stub implementation for response
   79:     // Stub implementation for websocket
   99:     // Stub implementation for middleware context
  107:     // Minimal stub representing a route rule returned by CROW_ROUTE and friends
  166:             // Stub implementation that returns a default-constructed T
  185:             // Stub implementation that returns a default-constructed T
  196:     // Stub implementation for Crow app (alias)
  210:     // HTTP parser stubs
  211:     namespace http_parser_stub {
  252:     }  // namespace http_parser_stub

include/crow/http_parser_fixed.h:
   4: // It provides stub implementations for HTTP parser functionality
  16:     namespace http_parser_stub {
  25:         // Stub implementation for http_parser_init
  43:         // Stub implementation for http_parser_execute
  51:         // Stub implementation for http_parser_url_init
  66:         // Stub implementation for http_parser_parse_url
  73:         // Stub implementation for http_errno_name
  78:         // Stub implementation for http_errno_description
  83:         // Stub implementation for http_method_str
  87:     }  // namespace http_parser_stub

include/crow/http_request.h:
  4: // It provides stub implementations for HTTP request functionality

include/crow/http_response.h:
  4: // It provides stub implementations for HTTP response functionality

include/crow/logging.h:
   4: // It provides stub implementations for logging functionality
  23:             // For now, we'll just provide a stub
  34:             // For now, we'll just provide a stub
  40:             // For now, we'll just provide a stub

include/crow/socket_adaptors.h:
    3: // This is a minimal stub version of socket_adaptors.h for the Crow framework
   23:     // Use our stub asio implementation
   24:     using tcp = asio_stub::ip::tcp;
   29:         SocketAdaptor(asio_stub::io_context& io_context, context*) : socket_() {}
   31:         asio_stub::io_context& get_io_context() { 
   32:             static asio_stub::io_context ctx;
   49:             socket_.shutdown(asio_stub::socket_base::shutdown_both, ec);
   55:             socket_.shutdown(asio_stub::socket_base::shutdown_send, ec);
   61:             socket_.shutdown(asio_stub::socket_base::shutdown_receive, ec);
   75:         using context = asio_stub::ssl::context;
   76:         using ssl_socket_t = asio_stub::ssl::stream<tcp::socket>;
   78:         SSLAdaptor(asio_stub::io_context& io_context, context* ctx) : 
  100:                 raw_socket().shutdown(asio_stub::socket_base::shutdown_both, ec);
  108:                 raw_socket().shutdown(asio_stub::socket_base::shutdown_send, ec);
  116:                 raw_socket().shutdown(asio_stub::socket_base::shutdown_receive, ec);
  121:         asio_stub::io_context& get_io_context() { 
  122:             static asio_stub::io_context ctx;
  128:             ssl_socket_->async_handshake(asio_stub::ssl::server, [f](const error_code& ec) { f(ec); });

include/memory/spdlog_isolation.h:
   41: // It provides stub implementations for spdlog functionality that can be
   46: // When compiling with CUDA, provide stub implementations
  192: // Stub implementations for spdlog
  209: // Stub for logger class
  245: // Stub for registry

src/blender/CMakeLists.txt:
  52: # implementation falls back to stub definitions via preprocessor checks.

src/compat/core.cu:
  102:   // Simple implementation for stub
  125:   // Simple implementation for stub

src/compat/cuda_api.cu:
  61:     // Simple implementation for stub - actual implementation would configure and launch real CUDA kernel
  85:     // Simple implementation for stub - actual implementation would configure and launch real CUDA kernel

tests/cuda/utils_test.cpp:
  51: // Note: Removed logCudaError stub since it's defined in the actual utils.cu file
