#pragma once

// This is a minimal version of the logging.h file from the Crow framework
// It provides stub implementations for logging functionality

// Use relative path from project root
#include "compat/shim.h"
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
        void operator()(const sep::shim::string& message) {
            // In a real implementation, this would log to stderr
            // For now, we'll just provide a stub
        }
    };

    class Logger {
    public:
        Logger(LogLevel level) : level_(level) {}

        template <typename T>
        Logger& operator<<(T const& value) {
            // In a real implementation, this would append to a message
            // For now, we'll just provide a stub
            return *this;
        }

        ~Logger() {
            // In a real implementation, this would flush the log message
            // For now, we'll just provide a stub
        }

    private:
        LogLevel level_;
        // Using a simple string instead of stringstream to avoid template issues
        sep::shim::string message_;
        LogHandler handler_;
    };
}

// Define logging macros - use non-template version to avoid template argument issues
#define CROW_LOG_ERROR crow::Logger(crow::LogLevel::Error)
#define CROW_LOG_WARNING crow::Logger(crow::LogLevel::Warning)
#define CROW_LOG_INFO crow::Logger(crow::LogLevel::Info)
#define CROW_LOG_DEBUG crow::Logger(crow::LogLevel::Debug)