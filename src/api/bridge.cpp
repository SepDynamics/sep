#define BUILDING_SEP_BRIDGE
#include "api/bridge.h"
#include "api/bridge.hpp"
#include "quantum/processor.h"
#include "api/types.h"


#include <mutex>
#include <string>
#include <memory>
#include <exception>
#include <unordered_map>
#include <vector>
#include <nlohmann/json.hpp>

#if defined(__cpp_exceptions) || defined(__EXCEPTIONS) || defined(_CPPUNWIND)
#define SEP_HAS_EXCEPTIONS 1
#else
#define SEP_HAS_EXCEPTIONS 0
#endif

#if !SEP_HAS_EXCEPTIONS
#include "crow/crow_error.h"
#endif

#if SEP_HAS_EXCEPTIONS
#define SEP_TRY try
#define SEP_CATCH_RETURN(core) \
  catch (const std::exception &e) { \
    sep::api::bridge::detail::setLastError(e.what()); \
    return static_cast<int>(sep::api::bridge::detail::mapSepError(core)); \
  }
#else
#define SEP_TRY
#define SEP_CATCH_RETURN(core) \
  do { sep::crow::error::set_last_error("exceptions disabled"); \
    return static_cast<int>(core); \
  } while (0)
#endif

namespace sep::api::bridge::detail {
std::unique_ptr<sep::quantum::Processor> g_context_processor_bridge;
std::string g_last_error;
size_t g_required_buffer_size = 0;
// Global mutex protects shared bridge state
std::mutex g_bridge_mutex;  // Mutex for thread safety
std::unordered_map<std::string, std::vector<void (*)(const char *)>>
    g_callback_map;
} // namespace sep::api::bridge::detail

namespace sep::api::bridge::detail {

void setLastError(const std::string& error) {
  g_last_error = error;
#if !SEP_HAS_EXCEPTIONS
  sep::crow::error::set_last_error(error.c_str());
#endif
}
std::string getLastError() { return g_last_error; }

void setRequiredBufferSize(size_t size) { g_required_buffer_size = size; }

size_t getRequiredBufferSize() { return g_required_buffer_size; }

sep::api::ErrorCode mapSepError(sep::api::ErrorCode core) {
  switch (core) {
    case sep::api::ErrorCode::InvalidArgument:
      return sep::api::ErrorCode::InvalidParameter;
    case sep::api::ErrorCode::CudaError:
    case sep::api::ErrorCode::ApiError:
    case sep::api::ErrorCode::InvalidOperation:
    case sep::api::ErrorCode::ResourceNotFound:
    case sep::api::ErrorCode::OutOfMemory:
    case sep::api::ErrorCode::InvalidState:
    case sep::api::ErrorCode::SystemError:
    case sep::api::ErrorCode::Unknown:
      return sep::api::ErrorCode::ProcessingError;
    default:
      return sep::api::ErrorCode::GeneralError;
  }
}

} // namespace sep::api::bridge::detail

namespace sep::api::bridge {

nlohmann::json contextToJson(const sep::context::Context &context) {
  nlohmann::json json;
  json["type"] = std::string(context.type.c_str());
  json["content"] = context.content;
  json["relationships"] = std::vector<nlohmann::json>(context.relationships.begin(),
                                                      context.relationships.end());
  std::vector<std::string> tags;
  tags.reserve(context.tags.size());
  for (const auto &t : context.tags) {
    tags.emplace_back(t.c_str());
  }
  json["tags"] = tags;
  json["metadata"] = context.metadata;
  json["processorResult"] = context.processorResult;
  return json;
}

sep::context::Context jsonToContext(const nlohmann::json &json) {
  sep::context::Context context;
  auto type_str = json.value("type", std::string{});
  context.type = std::string(type_str.c_str());
  context.content = json.value("content", nlohmann::json{});

  auto rels = json.value("relationships", std::vector<nlohmann::json>{});
  context.relationships.clear();
  context.relationships.reserve(rels.size());
  for (const auto &r : rels) {
    context.relationships.push_back(r);
  }

  auto tag_vec = json.value("tags", std::vector<std::string>{});
  context.tags.clear();
  context.tags.reserve(tag_vec.size());
  for (const auto &t : tag_vec) {
    context.tags.push_back(std::string(t.c_str()));
  }

  context.metadata = json.value("metadata", nlohmann::json{});
  context.processorResult = json.value("processorResult", nlohmann::json{});
  return context;
}

nlohmann::json resultToJson(const sep::context::CheckResult &result) {
  nlohmann::json json;
  json["status"] = static_cast<int>(result.status);
  json["score"] = result.score;
  if (!result.error.empty()) {
    json["error"] = std::string(result.error.c_str());
  }
  return json;
}

sep::context::CheckResult jsonToCheckResult(const nlohmann::json &json) {
  sep::context::CheckResult result;
  result.status =
      static_cast<sep::context::CheckResult::Status>(json.value("status", 0));
  result.score = json.value("score", 0.0f);
  auto err = json.value("error", std::string{});
  result.error = std::string(err.c_str());
  return result;
}

} // namespace sep::api::bridge

namespace sep::api::bridge::detail {

void invokeCallbacks(const std::string &event_type,
                     const std::string &event_data) {
  std::lock_guard<std::mutex> lock(g_bridge_mutex);
  auto it = g_callback_map.find(event_type);
  if (it == g_callback_map.end()) {
    return;
  }
  for (auto &cb : it->second) {
    if (cb) {
      cb(event_data.c_str());
    }
  }
}

} // namespace sep::api::bridge::detail
