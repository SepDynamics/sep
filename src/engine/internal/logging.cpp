#include "logging.h"

#include "common.h"

#ifdef SEP_HAS_OPENTELEMETRY
#include <opentelemetry/trace/provider.h>
#endif

#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <cstring> // For std::strlen
#include <spdlog/spdlog.h>
#include <cstring>
#include "quantum/pattern_metric_engine.h"

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
  spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%^%l%$] %v");
  spdlog::set_level(spdlog::level::info);
}

void Manager::shutdown() { spdlog::shutdown(); }

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
                                                      const LoggerConfig &config)
{
    auto logger = spdlog::get(name.c_str());
    if (logger && !logger->sinks().empty())
    {
        return logger;
    }

    std::vector<spdlog::sink_ptr> sinks;

    if (config.console.enabled)
    {
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_level(toSpdLogLevel(config.level));
        sinks.push_back(console_sink);
    }

    if (!config.file.path.empty())
    {
        auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
            config.file.path.c_str(), config.file.max_size, config.file.max_files);
        file_sink->set_level(toSpdLogLevel(config.level));
        sinks.push_back(file_sink);
    }

    logger = std::make_shared<spdlog::logger>(name.c_str(), sinks.begin(), sinks.end());
    logger->set_level(toSpdLogLevel(config.level));
    if (!config.pattern.empty())
    {
        logger->set_pattern(config.pattern.c_str());
    }
    spdlog::register_logger(logger);

    return logger;
}

std::shared_ptr<spdlog::logger> Manager::getLogger(const std::string &name)
{
    return spdlog::get(name.c_str());
}

void Manager::setGlobalLevel(Level level) { spdlog::set_level(toSpdLogLevel(level)); }

Level Manager::levelFromString(const std::string &level)
{
    if (level == "trace") return Level::TRACE;
    if (level == "debug") return Level::DEBUG;
    if (level == "info") return Level::INFO;
    if (level == "warn") return Level::WARN;
    if (level == "error") return Level::ERROR;
    if (level == "critical") return Level::CRITICAL;
    return Level::INFO;
}

std::string Manager::levelToString(Level level)
{
    switch (level)
    {
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

void logPatternDetected(const std::string &pattern_id,
                        std::chrono::system_clock::time_point timestamp)
{
    auto logger = spdlog::get("pattern_engine");
    if (!logger) return;

    std::time_t tt = std::chrono::system_clock::to_time_t(timestamp);
    char buf[32];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&tt));
    logger->info("Pattern detected {} at {}", pattern_id, buf);
}

void logSignalDetected(const std::string &pattern_id,
                       sep::quantum::SignalType type,
                       std::chrono::system_clock::time_point timestamp)
{
    auto logger = spdlog::get("pattern_engine");
    if (!logger) return;

    std::time_t tt = std::chrono::system_clock::to_time_t(timestamp);
    char buf[32];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&tt));

    const char* type_str = (type == sep::quantum::SignalType::BUY)
                               ? "BUY"
                               : (type == sep::quantum::SignalType::SELL ? "SELL" : "HOLD");

    logger->info("Signal {} for pattern {} at {}", type_str, pattern_id, buf);
}
void logTrade(const std::string &instrument,
              double units,
              double price,
              double pnl)
{
    auto logger = spdlog::get("trade_manager");
    if (!logger) return;
    logger->info("Trade {} {:.0f} @ {:.5f} pnl {:.5f}",
                 instrument,
                 units,
                 price,
                 pnl);
}

void logAnomaly(const std::string &message)
{
    auto logger = spdlog::get("trade_manager");
    if (!logger) return;
    logger->warn("Anomaly detected: {}", message);
}

}  // namespace sep::logging

