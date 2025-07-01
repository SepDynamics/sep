#ifndef SEP_LOGGING_MANAGER_H
#define SEP_LOGGING_MANAGER_H

#include "memory/types.h"
#include "core/tracing.h"
#include <spdlog/spdlog.h>
#include <memory>
#include <string>

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
  ::spdlog::level::level_enum toSpdLogLevel(Level level);
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

}  // namespace sep::logging

#endif  // SEP_LOGGING_MANAGER_H
