#pragma once

#include "compat/shim.h"
#include "memory/spdlog_isolation.h"
#include <memory>
#include <sstream>
#if SEP_HAS_SPDLOG
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#endif

namespace crow {

enum class LogLevel { Debug = 0, Info, Warning, Error, Critical };

namespace detail {
inline sep::spdlog::level to_spd_level(LogLevel lvl) {
    using SepLevel = sep::spdlog::level;
    switch (lvl) {
        case LogLevel::Debug: return SepLevel::debug;
        case LogLevel::Info: return SepLevel::info;
        case LogLevel::Warning: return SepLevel::warn;
        case LogLevel::Error: return SepLevel::err;
        case LogLevel::Critical: return SepLevel::critical;
    }
    return SepLevel::info;
}

inline std::shared_ptr<sep::spdlog::logger> get_logger() {
    auto logger = sep::spdlog::details::registry::instance().get("crow");
#if SEP_HAS_SPDLOG
    if (!logger) {
        logger = std::make_shared<sep::spdlog::logger>("crow", std::make_shared<::spdlog::sinks::stderr_sink_mt>());
        sep::spdlog::details::registry::instance().register_logger(logger);
    }
#endif
    return logger;
}
} // namespace detail

class LogHandler {
public:
    LogHandler() : logger_(detail::get_logger()) {}
    void operator()(const std::string& message, LogLevel level) {
#if SEP_HAS_SPDLOG
        logger_->log(detail::to_spd_level(level), message);
#else
        (void)message;
        (void)level;
#endif
    }
private:
    std::shared_ptr<sep::spdlog::logger> logger_;
};

class Logger {
public:
    explicit Logger(LogLevel level) : level_(level) {}

    template <typename T>
    Logger& operator<<(const T& value) {
        stream_ << value;
        return *this;
    }

    ~Logger() { handler_(stream_.str(), level_); }

private:
    LogLevel   level_;
    std::ostringstream stream_;
    LogHandler handler_;
};

} // namespace crow

#define CROW_LOG_ERROR   crow::Logger(crow::LogLevel::Error)
#define CROW_LOG_WARNING crow::Logger(crow::LogLevel::Warning)
#define CROW_LOG_INFO    crow::Logger(crow::LogLevel::Info)
#define CROW_LOG_DEBUG   crow::Logger(crow::LogLevel::Debug)

