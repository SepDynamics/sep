#pragma once

// Minimal logging helpers used by tests and small utilities.
// These wrappers route Crow style log calls through spdlog so that
// log output behaves consistently with the rest of the project.

#include "compat/shim.h"
#include "memory/spdlog_isolation.h"
#include <sstream>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <memory>

namespace crow {
    enum class LogLevel {
        Debug = 0,
        Info,
        Warning,
        Error,
        Critical
    };

    class LogHandler {
    public:
        explicit LogHandler(std::shared_ptr<sep::spdlog::logger> logger = sep::spdlog::details::registry::instance().get("crow"))
            : logger_(std::move(logger))
        {
            if (!logger_)
            {
                logger_ = ::spdlog::stderr_color_mt("crow");
            }
        }

        void operator()(LogLevel level, const sep::shim::string& message) const
        {
            logger_->log(toSpdLevel(level), message.c_str());
        }

    private:
        static sep::spdlog::level toSpdLevel(LogLevel level)
        {
            switch (level)
            {
                case LogLevel::Debug:    return sep::spdlog::level::debug;
                case LogLevel::Info:     return sep::spdlog::level::info;
                case LogLevel::Warning:  return sep::spdlog::level::warn;
                case LogLevel::Error:    return sep::spdlog::level::err;
                case LogLevel::Critical: return sep::spdlog::level::critical;
            }
            return sep::spdlog::level::info;
        }

        std::shared_ptr<sep::spdlog::logger> logger_;
    };

    class Logger {
    public:
        explicit Logger(LogLevel level) : level_(level) {}

        template <typename T>
        Logger& operator<<(const T& value)
        {
            ss_ << value;
            return *this;
        }

        ~Logger()
        {
            handler_(level_, ss_.str().c_str());
        }

    private:
        LogLevel level_;
        std::ostringstream ss_;
        LogHandler        handler_;
    };
}

// Define logging macros - use non-template version to avoid template argument issues
#define CROW_LOG_ERROR crow::Logger(crow::LogLevel::Error)
#define CROW_LOG_WARNING crow::Logger(crow::LogLevel::Warning)
#define CROW_LOG_INFO crow::Logger(crow::LogLevel::Info)
#define CROW_LOG_DEBUG crow::Logger(crow::LogLevel::Debug)
