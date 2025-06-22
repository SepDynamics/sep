#ifndef SEP_LOGGING_MANAGER_H
#define SEP_LOGGING_MANAGER_H



#include <chrono>
#include <memory>
#include <string>

#include "memory/types.h"
#include "metrics/tracing.h"

// Handle ASIO/Crow includes based on RTTI availability
#ifndef CROW_DISABLE_RTTI
// Use real headers in non-CUDA mode
#include "api/types.h"
#include <spdlog/spdlog.h>
#else
// Use isolation headers in CUDA mode
#include "crow/crow_isolation.h"
#include <spdlog/spdlog.h>
#endif

#include "api/server.h"
namespace sep {
namespace api {
class HttpRequest;
std::unique_ptr<HttpRequest> makeRequest(::crow::request &req);
}  // namespace api
}  // namespace sep

namespace sep::logging {

#ifndef SEP_HAS_OPENTELEMETRY
// Minimal tracer that creates TraceSpan instances for instrumentation without
// relying on OpenTelemetry. It exposes a `startSpan` helper mirroring the
// OpenTelemetry API shape in a limited form.
class SimpleTracer {
 public:
  template <typename... Args>
  std::unique_ptr<sep::metrics::TraceSpan> startSpan(Args&&... args) {
    return std::make_unique<sep::metrics::TraceSpan>(std::forward<Args>(args)...);
  }
};
#endif

class Manager {
 public:
  static Manager &getInstance() {
    static Manager instance;
    return instance;
  }

  // Delete copy operations
  Manager(const Manager &) = delete;
  Manager &operator=(const Manager &) = delete;

  // Logger operations
  std::shared_ptr<spdlog::logger> createLogger(const std::string &name, const LoggerConfig &config);
  std::shared_ptr<spdlog::logger> getLogger(const std::string &name);
  void setGlobalLevel(Level level);

  // Retrieve tracer. When OpenTelemetry is not compiled in, a lightweight
  // internal tracer is returned instead of `nullptr`.
  static void *getTracer();

  // Level conversion
  Level levelFromString(const std::string &level);
  std::string levelToString(Level level);

  // Initialize/shutdown
  static void initialize();
  static void shutdown();

 protected:
  Manager() = default;

 private:
  spdlog::level::level_enum toSpdLogLevel(Level level);
};

// Global functions
inline void initializeLogging() { Manager::initialize(); }
inline void shutdownLogging() { Manager::shutdown(); }
inline Level levelFromString(const std::string &level) {
  return Manager::getInstance().levelFromString(level);
}
inline std::string levelToString(Level level) {
  return Manager::getInstance().levelToString(level);
}

// Middleware for HTTP request logging
class LoggingMiddleware {
 public:
  struct context {
    std::chrono::high_resolution_clock::time_point start;
  };

  explicit LoggingMiddleware(api::Server *server) : server_(server) {}

  void before_handle(::crow::request &req, ::crow::response &res,
                     LoggingMiddleware::context &ctx);
  void after_handle(::crow::request &req, ::crow::response &res,
                    LoggingMiddleware::context &ctx);

 private:
  bool isReady() const { return server_ != nullptr; }
  api::Server *server_;
};

}  // namespace sep::logging

#endif  // SEP_LOGGING_MANAGER_H
