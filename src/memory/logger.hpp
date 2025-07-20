#pragma once

#include <fmt/format.h>
#include <memory>
#include <string>

namespace sep {
namespace logging {

class Logger
{
public:
    virtual ~Logger() = default;

    virtual void debug(const std::string& msg) = 0;
    virtual void info(const std::string& msg) = 0;
    virtual void warn(const std::string& msg) = 0;
    virtual void error(const std::string& msg) = 0;
    virtual void critical(const std::string& msg) = 0;

    template <typename... Args>
    void debug(const std::string& fmt, Args&&... args)
    {
        debug(fmt::format(fmt, std::forward<Args>(args)...));
    }

    template <typename... Args>
    void info(const std::string& fmt, Args&&... args)
    {
        info(fmt::format(fmt, std::forward<Args>(args)...));
    }

    template <typename... Args>
    void warn(const std::string& fmt, Args&&... args)
    {
        warn(fmt::format(fmt, std::forward<Args>(args)...));
    }

    template <typename... Args>
    void error(const std::string& fmt, Args&&... args)
    {
        error(fmt::format(fmt, std::forward<Args>(args)...));
    }

    template <typename... Args>
    void critical(const std::string& fmt, Args&&... args)
    {
        critical(fmt::format(fmt, std::forward<Args>(args)...));
    }
};

using LoggerPtr = std::shared_ptr<Logger>;

// Logging macros
#define LOG_DEBUG(logger, ...) \
    if (logger)                \
    logger->debug(__VA_ARGS__)
#define LOG_INFO(logger, ...) \
    if (logger)               \
    logger->info(__VA_ARGS__)
#define LOG_WARN(logger, ...) \
    if (logger)               \
    logger->warn(__VA_ARGS__)
#define LOG_ERROR(logger, ...) \
    if (logger)                \
    logger->error(__VA_ARGS__)
#define LOG_CRITICAL(logger, ...) \
    if (logger)                   \
    logger->critical(__VA_ARGS__)

}  // namespace logging
}  // namespace sep
