#include <atomic>

// Debug logging for OpenTelemetry headers
#ifdef SEP_HAS_OPENTELEMETRY
#include <opentelemetry/trace/provider.h>
#endif
// CROW_DISABLE_RTTI is defined globally via CMake
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/basic_file_sink.h> // Fix: Include basic file sink
#include <spdlog/sinks/daily_file_sink.h> // Fix: Include daily file sink
#include <spdlog/sinks/dist_sink.h> // Fix: Include distributed sink
#include <spdlog/sinks/null_sink.h> // Fix: Include null sink
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include "api/crow_adapter.h" // Fix: Include crow_adapter for LoggingMiddleware
#include "core/common.h" // Fix: Include common
#include "memory/memory_tier_manager.hpp"
#include "quantum/quantum_processor_qfh.h"
#include "memory/manager.h"

namespace sep::logging {

spdlog::level::level_enum Manager::toSpdLogLevel(Level level) {
  switch (level) {
    case Level::TRACE:
      return spdlog::level::trace;
    case Level::DEBUG:
      return spdlog::level::debug;
    case Level::INFO:
      return spdlog::level::info;
    case Level::WARN:
      return spdlog::level::warn;
    case Level::ERROR:
      return spdlog::level::err;
    case Level::CRITICAL:
      return spdlog::level::critical;
    default:
      return spdlog::level::info;
  }
}

void Manager::initialize() {
  // Set up default configuration
  spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%^%l%$] %v");
  spdlog::set_level(spdlog::level::info);

  // Tracing hooks removed
}

void Manager::shutdown() { spdlog::shutdown(); }

// Return tracer based on available backend. With OpenTelemetry built in, defer
// to the provider. Otherwise return a lightweight internal tracer.
void *Manager::getTracer() {
#ifdef SEP_HAS_OPENTELEMETRY
  static auto tracer = opentelemetry::trace::Provider::GetTracerProvider()
                           ->GetTracer("sep_logging");
  return tracer.get();
#else
  static SimpleTracer tracer;
  return &tracer;
#endif
}

std::shared_ptr<spdlog::logger> Manager::createLogger(const std::string &name,
                                                      const LoggerConfig &config) {
  auto logger = spdlog::get(name);
  // Check if a logger with this name already exists and if it has sinks.
  // If it exists but has no sinks, it was likely created by spdlog::get(name)
  // before we explicitly created it. We should replace it in this case.
  if (logger && !logger->sinks().empty()) {
    return logger;
  }

  std::vector<spdlog::sink_ptr> sinks;

  // Add console sink if enabled
  if (config.console.enabled) {
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(toSpdLogLevel(config.level));
    sinks.push_back(console_sink);
  }

  // Add file sink if path is provided
  if (!config.file.path.empty()) {
    auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
        config.file.path, config.file.max_size, config.file.max_files);
    file_sink->set_level(toSpdLogLevel(config.level));
    sinks.push_back(file_sink);
  }

  logger = std::make_shared<spdlog::logger>(name, sinks.begin(), sinks.end());
  logger->set_level(toSpdLogLevel(config.level));
  if (!config.pattern.empty()) {
    logger->set_pattern(config.pattern);
  }
  spdlog::register_logger(logger);

  return logger;
}

std::shared_ptr<spdlog::logger> Manager::getLogger(const std::string &name) {
  return spdlog::get(name);
}

void Manager::setGlobalLevel(Level level) { spdlog::set_level(toSpdLogLevel(level)); }

Level Manager::levelFromString(const std::string &level) {
  if (level == "trace") return Level::TRACE;
  if (level == "debug") return Level::DEBUG;
  if (level == "info") return Level::INFO;
  if (level == "warn") return Level::WARN;
  if (level == "error") return Level::ERROR;
  if (level == "critical") return Level::CRITICAL;
  return Level::INFO;  // Default to info
}

std::string Manager::levelToString(Level level) {
  switch (level) {
    case Level::TRACE:
      return "trace";
    case Level::DEBUG:
      return "debug";
    case Level::INFO:
      return "info";
    case Level::WARN:
      return "warn";
    case Level::ERROR:
      return "error";
    case Level::CRITICAL:
      return "critical";
    default:
      return "info";
  }
}

void LoggingMiddleware::before_handle(::crow::request& req, ::crow::response& res,
                                      LoggingMiddleware::context &ctx) {

  (void)req;
  if (!isReady()) { // Fix: Added comment // Fix: Added comment
    res.code = 503;  // Service Unavailable
    res.end();
    return;
  }

  ctx.start = std::chrono::high_resolution_clock::now();
  // Ensure ctx.start write is visible before any other thread reads it. The
  // middleware currently runs sequentially but this guard preserves ordering if
  // the server is extended with asynchronous handlers.
  std::atomic_thread_fence(std::memory_order_release);

}

void LoggingMiddleware::after_handle(::crow::request &req, ::crow::response &res,
                                     LoggingMiddleware::context &ctx) {

  if (!isReady()) {
    return;
  }

  auto req_ptr = std::make_unique<sep::api::CrowRequestAdapter>(req);

  if (ctx.start != std::chrono::high_resolution_clock::time_point{}) {
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - ctx.start);
    server_->logRequest(*req_ptr, res.code, res.body, duration.count());
  } else {
    server_->logRequest(*req_ptr, res.code, res.body, 0);
  }
}

}  // namespace sep::logging
