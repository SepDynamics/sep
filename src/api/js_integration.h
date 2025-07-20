#ifndef SEP_API_JS_INTEGRATION_H
#define SEP_API_JS_INTEGRATION_H

#include <string>

#include "api/bridge.h"

namespace sep::api {

class JSIntegration {
public:
  static constexpr size_t initial_buffer_size = 4096;

  /**
   * @brief Process a context check request from JavaScript
   * @param context_json JSON string containing the context to check
   * @param layer Layer name to check against
   * @return JSON string containing the check result
   */
  static shim::string processContextCheck(const shim::string &context_json,
                                          const shim::string &layer);
};

} // namespace sep::api

#endif // SEP_API_JS_INTEGRATION_H
