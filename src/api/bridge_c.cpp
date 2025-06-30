#include <cstring>
#include <cstdio>
#include <memory>
#include <mutex>

#include <nlohmann/json.hpp>

#include "api/bridge.h"
#include "quantum/types.h"
#include "api/bridge_internal.hpp"
#include "core/manager.h"
#include "core/common.h"  // defines sep::SEPResult
#include "compat/cuda_helpers.h"
#include "compat/shim.h"
#include "compat/cuda_runtime.h"  // For sep::cuda::cudaMemcpyAsync
#include "compat/macros.h"  // For SEP_CUDA_AVAILABLE
#include "crow/asio_isolation.h"
#include "crow/socket_adaptors.h"
#include <ios>
#include <nlohmann/json.hpp>
#include <cstring>
#include <cstdio>  // Required for snprintf
#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>


extern "C" {

SEP_API sep::SEPResult sep_bridge_init(void) {
#ifdef SEP_HAS_EXCEPTIONS
  try {
#endif
  std::lock_guard<std::mutex> lock(sep::api::bridge::detail::g_bridge_mutex);
  sep::quantum::ProcessingConfig options{};
  sep::api::bridge::detail::g_context_processor_bridge = sep::quantum::createProcessor(options);
  sep::api::bridge::detail::g_last_error.clear();
  // Initialize g_required_buffer_size to 0 here
  sep::api::bridge::detail::g_required_buffer_size = 0;
  return sep::SEPResult::SUCCESS;
#if SEP_HAS_EXCEPTIONS
  } SEP_CATCH_RETURN(sep::api::ErrorCode::ApiError);
}

SEP_API sep::SEPResult sep_bridge_cleanup(void) {
  std::lock_guard<std::mutex> lock(sep::api::bridge::detail::g_bridge_mutex);
  sep::api::bridge::detail::g_context_processor_bridge.reset();
  sep::api::bridge::detail::g_last_error.clear();
  // Initialize g_required_buffer_size to 0 here
  sep::api::bridge::detail::g_required_buffer_size = 0;
  return sep::SEPResult::SUCCESS;
}

SEP_API sep::SEPResult sep_process_context(const char *context_json, const char *layer,
                               char *result_buffer, size_t buffer_size) {
#if SEP_HAS_EXCEPTIONS
  try {
#endif
    if (!context_json || !result_buffer || !layer || buffer_size == 0) {
      sep::api::bridge::detail::setLastError("Invalid parameters");
      return sep::SEPResult::INVALID_ARGUMENT;
    }

    sep::quantum::Processor *processor = nullptr;
    {
      std::lock_guard<std::mutex> lock(sep::api::bridge::detail::g_bridge_mutex);
      if (!sep::api::bridge::detail::g_context_processor_bridge) {
        sep::api::bridge::detail::setLastError("Context processor not initialized");
        return sep::SEPResult::UNKNOWN_ERROR;
      }
      processor = sep::api::bridge::detail::g_context_processor_bridge.get();
    }

    {
      nlohmann::json json_obj = nlohmann::json::parse(context_json, nullptr, false);
      if (json_obj.is_discarded()) {
        sep::api::bridge::detail::setLastError("JSON parsing error");
        return sep::SEPResult::PROCESSING_ERROR;
      }

      nlohmann::json test_result;
      // Dummy processing result
      test_result["success"] = true; 
      test_result["results"] = nlohmann::json::array(); 
      
      std::string test_str = test_result.dump();
      {
        std::lock_guard<std::mutex> lock(sep::api::bridge::detail::g_bridge_mutex);
        sep::api::bridge::detail::setRequiredBufferSize(test_str.size() + 1);
        if (test_str.size() >= buffer_size) {
          sep::api::bridge::detail::setLastError("Result buffer too small");
          return sep::SEPResult::BUFFER_TOO_SMALL;
        }
      }

      sep::quantum::BatchProcessingResult process_result;
      process_result.success = true;
      process_result.error_code = 0;
      
      if (!process_result.success) {
        sep::api::bridge::detail::setLastError(process_result.error_message.c_str());
        return sep::SEPResult::PROCESSING_ERROR;
      }

      nlohmann::json result_json;
      result_json["success"] = true;
      result_json["results"] = nlohmann::json::array();
      for (const auto &processing_result : process_result.results) {
        nlohmann::json result_entry;
        result_entry["success"] = processing_result.success;
        result_entry["pattern"] = processing_result.pattern.id;
        if (!processing_result.success) {
          result_entry["error"] = processing_result.error_message;
        }
        result_json["results"].push_back(result_entry);
      }

      std::string result_str = result_json.dump();
        {
          std::lock_guard<std::mutex> lock(sep::api::bridge::detail::g_bridge_mutex);
          sep::api::bridge::detail::setRequiredBufferSize(result_str.size() + 1);
        if (result_str.size() >= buffer_size) {
          sep::api::bridge::detail::setLastError("Result buffer too small");
          return sep::SEPResult::BUFFER_TOO_SMALL;
        }
      }

      (void)std::snprintf(result_buffer, buffer_size, "%s", result_str.c_str());
      return sep::SEPResult::SUCCESS;
    }
#ifdef SEP_HAS_EXCEPTIONS
  } catch (const std::exception &e) {
    sep::api::bridge::detail::setLastError(e.what());
    return sep::SEPResult::PROCESSING_ERROR;
  }
#endif
}

SEP_API sep::SEPResult sep_bridge_get_last_error(char *buffer, size_t buffer_size) {
  std::lock_guard<std::mutex> lock(sep::api::bridge::detail::g_bridge_mutex);
  if (!buffer || buffer_size == 0) { // Fix: Check for null buffer or zero size
    return sep::SEPResult::INVALID_ARGUMENT;
  }
  size_t len = std::min(sep::api::bridge::detail::g_last_error.size(), buffer_size - 1);
  (void)std::snprintf(buffer, buffer_size, "%s", sep::api::bridge::detail::g_last_error.c_str());
  (void)len; // length currently unused
  return sep::SEPResult::SUCCESS;
}

SEP_API size_t sep_get_required_buffer_size(void) {
#if SEP_HAS_EXCEPTIONS
  try {
#endif
  std::lock_guard<std::mutex> lock(sep::api::bridge::detail::g_bridge_mutex);
  return sep::api::bridge::detail::g_required_buffer_size;
#if SEP_HAS_EXCEPTIONS
  } SEP_CATCH_RETURN(sep::api::ErrorCode::GeneralError);
#endif
}

SEP_API sep::SEPResult sep_bridge_set_config(const char *key, const char *value) {
#if SEP_HAS_EXCEPTIONS
  try {
#endif
  std::lock_guard<std::mutex> lock(sep::api::bridge::detail::g_bridge_mutex);
  if (!key || !value) {
    sep::api::bridge::detail::setLastError("Invalid parameters");
    return sep::SEPResult::UNKNOWN_ERROR;
  }
  std::string k = key;
  auto &cm = sep::config::ConfigManager::getInstance();
  auto cfg = cm.getAPIConfig();
  try {
    if (k == "api.host") {
      cfg.host = value;
    } else if (k == "api.port") {
      cfg.port = static_cast<uint16_t>(std::stoi(value));
    } else if (k == "api.threads") {
      cfg.threads = static_cast<uint32_t>(std::stoul(value));
    } else if (k == "api.log_level") {
      cfg.log_level = value;
    } else if (k == "api.enable_metrics") {
      sep::shim::string v = value;
      cfg.enable_metrics = (v == "1" || v == "true");
    } else if (k == "api.keep_alive_timeout_ms") {
      cfg.keep_alive_timeout_ms = static_cast<size_t>(std::stoul(value));
    } else {
      sep::api::bridge::detail::setLastError("Config key not found");
      return sep::SEPResult::INVALID_ARGUMENT;
    }
  } catch (...) {
    sep::api::bridge::detail::setLastError("Invalid value");
    return sep::SEPResult::UNKNOWN_ERROR;
  }
  cm.updateAPIConfig(cfg);
  sep::api::bridge::detail::setLastError("");
 return sep::SEPResult::SUCCESS; // Fix: Add semicolon
#if SEP_HAS_EXCEPTIONS
  } SEP_CATCH_RETURN(sep::api::ErrorCode::GeneralError);
}

SEP_API sep::SEPResult sep_bridge_get_config(const char *key, char *buffer, size_t buffer_size) {
#if SEP_HAS_EXCEPTIONS
  try {
#endif
  std::lock_guard<std::mutex> lock(sep::api::bridge::detail::g_bridge_mutex);
  if (!key || !buffer || buffer_size == 0) {
    sep::api::bridge::detail::setLastError("Invalid parameters");
    return sep::SEPResult::UNKNOWN_ERROR;
  }
  std::string k = key;
  const auto &cfg = sep::config::ConfigManager::getInstance().getAPIConfig();
  std::string val;
  if (k == "api.host") {
    val = cfg.host;
  } else if (k == "api.port") {
    val = std::to_string(cfg.port);
  } else if (k == "api.threads") {
    val = std::to_string(cfg.threads);
  } else if (k == "api.log_level") {
    val = cfg.log_level;
  } else if (k == "api.enable_metrics") {
    val = cfg.enable_metrics ? "true" : "false";
  } else if (k == "api.keep_alive_timeout_ms") {
    val = std::to_string(cfg.keep_alive_timeout_ms);
  } else {
    buffer[0] = '\0';
    sep::api::bridge::detail::setLastError("Config key not found");
    return sep::SEPResult::INVALID_ARGUMENT;
  }
  (void)std::snprintf(buffer, buffer_size, "%s", val.c_str());
  sep::api::bridge::detail::setLastError("");
 return sep::SEPResult::SUCCESS; // Fix: Add semicolon
#if SEP_HAS_EXCEPTIONS
  } SEP_CATCH_RETURN(sep::api::ErrorCode::GeneralError);
}

SEP_API sep::SEPResult sep_bridge_register_callback(const char *event_type,
                                         void (*callback)(const char *event_data)) {
#if SEP_HAS_EXCEPTIONS
  try {
#endif
  std::lock_guard<std::mutex> lock(sep::api::bridge::detail::g_bridge_mutex);
  if (!event_type || !callback) {
    sep::api::bridge::detail::setLastError("Invalid parameters");
    return sep::SEPResult::UNKNOWN_ERROR;
  } // Fix: Add closing brace
  sep::api::bridge::detail::g_callback_map[event_type].push_back(callback);
  sep::api::bridge::detail::setLastError("");
 return sep::SEPResult::SUCCESS; // Fix: Add semicolon
#if SEP_HAS_EXCEPTIONS
  } SEP_CATCH_RETURN(sep::api::ErrorCode::GeneralError);
}

} // extern "C"
