#include <cuda_runtime.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <memory>
#include <mutex>
#include <nlohmann/json.hpp>
#include <string>

#include "api/bridge.h"
#include "api/bridge_internal.hpp"
#include "crow/asio_isolation.h"
#include "crow/socket_adaptors.h"
#include "engine/common.h"
#include "engine/cuda_helpers.h"
#include "engine/macros.h"
#include "engine/manager.h"
#include "engine/standard_includes.h"
#include "engine/types.h"

extern "C"
{
    SEP_API sep::SEPResult sep_bridge_init(void)
    {
    #ifdef SEP_HAS_EXCEPTIONS
      try {
    #endif
        std::lock_guard<std::mutex> lock(sep::api::bridge::detail::g_bridge_mutex);
        sep::quantum::ProcessingConfig options{};
        sep::api::bridge::detail::g_context_processor_bridge = sep::quantum::createProcessor(options);
        sep::api::bridge::detail::g_last_error.clear();
        sep::api::bridge::detail::g_required_buffer_size = 0;
        return sep::SEPResult::SUCCESS;
    #ifdef SEP_HAS_EXCEPTIONS
      } catch (const std::exception& e) {
        sep::api::bridge::detail::setLastError(e.what());
        return sep::SEPResult::UNKNOWN_ERROR;
      }
    #endif
    }

SEP_API sep::SEPResult sep_bridge_cleanup(void) {
#ifdef SEP_HAS_EXCEPTIONS
  try {
#endif
    std::lock_guard<std::mutex> lock(sep::api::bridge::detail::g_bridge_mutex);
    sep::api::bridge::detail::g_context_processor_bridge.reset();
    sep::api::bridge::detail::g_last_error.clear();
    sep::api::bridge::detail::g_required_buffer_size = 0;
    return sep::SEPResult::SUCCESS;
#ifdef SEP_HAS_EXCEPTIONS
  } catch (const std::exception& e) {
    sep::api::bridge::detail::setLastError(e.what());
    return sep::SEPResult::UNKNOWN_ERROR;
  }
#endif
}

SEP_API sep::SEPResult sep_process_context(const char *context_json, const char *layer,
                                         char *result_buffer, size_t buffer_size)
{
#ifdef SEP_HAS_EXCEPTIONS
    try {
#endif
        if (!context_json || !result_buffer || !layer || buffer_size == 0) {
            sep::api::bridge::detail::setLastError("Invalid parameters");
            return sep::SEPResult::INVALID_ARGUMENT;
        }

        std::lock_guard<std::mutex> lock(sep::api::bridge::detail::g_bridge_mutex);
        
        if (!sep::api::bridge::detail::g_context_processor_bridge) {
            sep::api::bridge::detail::setLastError("Context processor not initialized");
            return sep::SEPResult::UNKNOWN_ERROR;
        }

        // Parse input JSON
        nlohmann::json json_obj = nlohmann::json::parse(context_json, nullptr, false);
        if (json_obj.is_discarded()) {
            sep::api::bridge::detail::setLastError("JSON parsing error");
            return sep::SEPResult::PROCESSING_ERROR;
        }

        // Parse input patterns from JSON
        std::vector<std::string> pattern_ids;
        for (const auto& pattern : json_obj["patterns"]) {
            pattern_ids.push_back(pattern["id"].get<std::string>());
        }

        // Process patterns through quantum pipeline
        sep::BatchProcessingResult process_result =
            sep::api::bridge::detail::g_context_processor_bridge->processBatch(pattern_ids);

        // Format results as JSON
        nlohmann::json result_json;
        result_json["success"] = process_result.success;
        result_json["results"] = nlohmann::json::array();

        if (!process_result.success) {
            result_json["error"] = process_result.error_message;
        } else {
            for (const auto& result : process_result.results) {
                nlohmann::json result_entry;
                result_entry["success"] = result.success;
                result_entry["pattern"] = result.pattern.id;
                result_entry["coherence"] = result.pattern.quantum_state.coherence;
                result_entry["stability"] = result.pattern.quantum_state.stability;
                result_entry["generation"] = result.pattern.generation;
                result_entry["quantum_generation"] = result.pattern.quantum_state.generation;
                
                if (!result.success) {
                    result_entry["error"] = result.error_message;
                }
                result_json["results"].push_back(result_entry);
            }
        }

        // Write results to output buffer
        std::string result_str = result_json.dump();
        sep::api::bridge::detail::setRequiredBufferSize(result_str.size() + 1);
        if (result_str.size() >= buffer_size) {
            sep::api::bridge::detail::setLastError("Result buffer too small");
            return sep::SEPResult::BUFFER_TOO_SMALL;
        }
        std::strncpy(result_buffer, result_str.c_str(), buffer_size - 1);
        result_buffer[buffer_size - 1] = '\0';

        return sep::SEPResult::SUCCESS;
#ifdef SEP_HAS_EXCEPTIONS
    } catch (const std::exception& e) {
        sep::api::bridge::detail::setLastError(e.what());
        return sep::SEPResult::UNKNOWN_ERROR;
    }
#endif
}
}  // extern "C"
