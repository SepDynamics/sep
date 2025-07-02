#pragma once

// This is a minimal version of the logging.h file from the Crow framework
// It provides stub implementations for logging functionality

// Use relative path from project root
#include "compat/shim.h"
#include "memory/spdlog_isolation.h"
#include <sstream>

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
        explicit LogHandler(spdlog::level::level_enum level,
                            std::shared_ptr<spdlog::logger> logger =
                                spdlog::default_logger())
            : level_(level), logger_(std::move(logger)) {}

        void operator()(const sep::shim::string& message) {
            if (logger_) {
                logger_->log(level_, message.c_str());
            } else {
                spdlog::log(level_, message.c_str());
            }
        }

    private:
        spdlog::level::level_enum           level_;
        std::shared_ptr<spdlog::logger> logger_;
    };

    class Logger {
    public:
        explicit Logger(LogLevel level,
                        std::shared_ptr<spdlog::logger> logger =
                            spdlog::default_logger())
            : level_(level), logger_(std::move(logger)) {}

        template <typename T>
        Logger& operator<<(T const& value) {
            message_ << value;
            return *this;
        }

        ~Logger() {
            if (logger_) {
                logger_->log(toSpd(level_), message_.str());
            } else {
                spdlog::log(toSpd(level_), message_.str());
            }
        }

    private:
        static spdlog::level::level_enum toSpd(LogLevel level) {
            switch (level) {
            case LogLevel::Debug:
                return spdlog::level::debug;
            case LogLevel::Info:
                return spdlog::level::info;
            case LogLevel::Warning:
                return spdlog::level::warn;
            case LogLevel::Error:
                return spdlog::level::err;
            case LogLevel::Critical:
                return spdlog::level::critical;
            default:
                return spdlog::level::info;
            }
        }

        LogLevel                                level_;
        std::shared_ptr<spdlog::logger>         logger_;
        std::ostringstream                      message_;
    };
}

// Define logging macros - use non-template version to avoid template argument issues
#define CROW_LOG_ERROR crow::Logger(crow::LogLevel::Error)
#define CROW_LOG_WARNING crow::Logger(crow::LogLevel::Warning)
#define CROW_LOG_INFO crow::Logger(crow::LogLevel::Info)
#define CROW_LOG_DEBUG crow::Logger(crow::LogLevel::Debug)
