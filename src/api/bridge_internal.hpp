#pragma once
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>
#include "quantum/processor.h"
#include "api/types.h" // For ErrorCode
// Forward declarations
namespace sep {
namespace context {
class Processor;
}
}

#if defined(__cpp_exceptions) || defined(__EXCEPTIONS) || defined(_CPPUNWIND)
#define SEP_HAS_EXCEPTIONS 1
#else
#define SEP_HAS_EXCEPTIONS 0
#endif

#if SEP_HAS_EXCEPTIONS
#define SEP_TRY try
#define SEP_CATCH_RETURN(code) \
  catch (const std::exception &e) { \
    sep::api::bridge::detail::setLastError(e.what()); \
    return static_cast<int>(sep::api::bridge::detail::mapSepError(code)); \
  }
#else
#define SEP_TRY if (true)
#define SEP_CATCH_RETURN(code) \
  do { \
    sep::api::bridge::detail::setLastError("exceptions disabled"); \
    return static_cast<int>(code); \
  } while (0)
#endif

namespace sep::api::bridge::detail {
extern std::unique_ptr<sep::quantum::Processor> g_context_processor_bridge;
extern std::string g_last_error;
extern size_t g_required_buffer_size;
extern std::mutex g_bridge_mutex;
extern std::unordered_map<std::string, std::vector<void (*)(const char *)>>
    g_callback_map;

void setLastError(const std::string& error);
std::string getLastError();
void setRequiredBufferSize(size_t size);
size_t getRequiredBufferSize();
::sep::api::ErrorCode mapSepError(::sep::api::ErrorCode code);
void invokeCallbacks(const std::string& event_type, const std::string& event_data);
} // namespace sep::api::bridge::detail
