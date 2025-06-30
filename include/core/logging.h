#ifndef SEP_LOGGING_MANAGER_H
#define SEP_LOGGING_MANAGER_H

#include "memory/types.h"
#include "core/tracing.h"

#include <memory>
#include <string>
#include <chrono>

// Handle ASIO/Crow includes based on RTTI availability
#ifndef CROW_DISABLE_RTTI
#include "api/types.h"
#include <spdlog/spdlog.h>
#else
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
class SimpleTracer {
 public:
  template <typename... Args>
  std::unique_ptr<metrics::TraceSpan> startSpan(Args&&... args) {
    return std::make_unique<metrics::TraceSpan>(std::forward<Args>(args)...);
  }
};
#endif

class Manager {
 public:
  static Manager &getInstance() {
    static Manager instance;
    return instance;
  }

  Manager(const Manager &) = delete;
  Manager &operator=(const Manager &) = delete;

  std::shared_ptr<spdlog::logger> createLogger(const std::string &name,
                                               const LoggerConfig &config);
  std::shared_ptr<spdlog::logger> getLogger(const std::string &name);
  void setGlobalLevel(Level level);

  static void *getTracer();

  Level levelFromString(const std::string &level);
  std::string levelToString(Level level);

  static void initialize();
  static void shutdown();

 protected:
  Manager() = default;

 private:
  ::spdlog::level::level_enum toSpdLogLevel(Level level);
};

inline void initializeLogging() { Manager::initialize(); }
inline void shutdownLogging() { Manager::shutdown(); }
inline Level levelFromString(const std::string &level) {
  return Manager::getInstance().levelFromString(level);
}
inline std::string levelToString(Level level) {
  return Manager::getInstance().levelToString(level);
}

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

