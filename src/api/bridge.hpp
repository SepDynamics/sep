#ifndef SEP_API_BRIDGE_HPP
#define SEP_API_BRIDGE_HPP

#include "api/types.h"
#include "quantum/processor.h"
#include "quantum/types.h"
#include "quantum/resource_predictor.h" // Provides context types
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

#include "api/bridge.h"

namespace sep::api::bridge {

// JSON conversion utilities
nlohmann::json contextToJson(const ::sep::context::Context &context);
::sep::context::Context jsonToContext(const nlohmann::json &json);
nlohmann::json resultToJson(const ::sep::context::CheckResult &result);
::sep::context::CheckResult jsonToCheckResult(const nlohmann::json &json);

// Internal bridge utilities
namespace detail {
void setLastError(const std::string &error);
std::string getLastError();
  void setRequiredBufferSize(size_t size);
  size_t getRequiredBufferSize();
  ::sep::api::ErrorCode mapSepError(::sep::api::ErrorCode core);
  void invokeCallbacks(const std::string &event_type,
                       const std::string &event_data);
} // namespace detail

} // namespace sep::api::bridge

extern "C" {
SEP_API int sep_bridge_init(void);
SEP_API int sep_bridge_cleanup(void);
SEP_API int sep_process_context(const char *context_json, const char *layer,
                                char *result_buffer, size_t buffer_size);
SEP_API int sep_bridge_get_last_error(char *buffer, size_t buffer_size);
SEP_API size_t sep_get_required_buffer_size(void);
SEP_API int sep_bridge_set_config(const char *key, const char *value);
SEP_API int sep_bridge_get_config(const char *key, char *buffer, size_t buffer_size);
SEP_API int sep_bridge_register_callback(const char *event_type,
                                         void (*callback)(const char *event_data));
} // extern "C"

#if defined(__cpp_exceptions) || defined(__EXCEPTIONS) || defined(_CPPUNWIND)
#define SEP_BRIDGE_TRY try
#define SEP_BRIDGE_CATCH(core)                                                   \
  catch (const std::exception &e) {                                             \
    sep::api::bridge::detail::setLastError(e.what());                            \
    return static_cast<int>(core);                                              \
  }                                                                             \
  catch (...) {                                                                 \
    sep::api::bridge::detail::setLastError("Unknown error");                   \
    return static_cast<int>(core);                                              \
  }
#else
#define SEP_BRIDGE_TRY if (true)
#define SEP_BRIDGE_CATCH(core)                                                   \
  {                                                                             \
    sep::api::bridge::detail::setLastError("exceptions disabled");              \
    return static_cast<int>(core);                                              \
  }
#endif

#endif // SEP_API_BRIDGE_HPP
