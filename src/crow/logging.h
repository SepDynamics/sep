#pragma once

// Minimal logging wrapper used by the Crow compatibility layer.
// The original Crow project relies on its own logger but for SEP we
// forward everything to spdlog so log output is consistent with the rest
// of the engine.  When spdlog is not available (e.g. CUDA compilation)
// the isolation headers provide light‑weight stubs.

#include "compat/shim.h"
#include "memory/spdlog_isolation.h"
#include <sstream>
#include <memory>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace crow {

    // Define the missing LogLevel enum
    enum class LogLevel {
        Debug,
        Info,
        Warning,
        Error,
        Critical
    };

    class LogHandler {
    public:
        LogHandler();
        void operator()(LogLevel level, const sep::shim::string& message);

    private:
        std::shared_ptr<::spdlog::logger> logger_;
    };

    class Logger {
    public:
        explicit Logger(LogLevel level);

        template <typename T>
        Logger& operator<<(T const& value) {
            stream_ << value;
            return *this;
        }

        ~Logger() {
            if (!stream_.str().empty())
            {
                handler_(level_, stream_.str().c_str());
            }
        }

    private:
        LogLevel level_;
        std::ostringstream stream_;
        LogHandler handler_;
    };

    // Implementation
    inline LogHandler::LogHandler()
        : logger_(::spdlog::get("crow"))
    {
        if (!logger_)
        {
            logger_ = ::spdlog::stderr_color_mt("crow");
            logger_->set_level(::spdlog::level::trace);
        }
    }

    inline void LogHandler::operator()(LogLevel lvl, const sep::shim::string& message)
    {
        if (logger_)
        {
            ::spdlog::level::level_enum slvl;
            switch (lvl)
            {
            case LogLevel::Debug:    slvl = ::spdlog::level::debug; break;
            case LogLevel::Info:     slvl = ::spdlog::level::info; break;
            case LogLevel::Warning:  slvl = ::spdlog::level::warn; break;
            case LogLevel::Error:    slvl = ::spdlog::level::err; break;
            case LogLevel::Critical: slvl = ::spdlog::level::critical; break;
            default:                 slvl = ::spdlog::level::info; break;
            }
            logger_->log(slvl, "{}", message.c_str());
        }
    }

    inline Logger::Logger(LogLevel level)
        : level_(level)
    {}

}

// Define logging macros - use non-template version to avoid template argument issues
#define CROW_LOG_ERROR crow::Logger(crow::LogLevel::Error)
#define CROW_LOG_WARNING crow::Logger(crow::LogLevel::Warning)
#define CROW_LOG_INFO crow::Logger(crow::LogLevel::Info)
#define CROW_LOG_DEBUG crow::Logger(crow::LogLevel::Debug)
