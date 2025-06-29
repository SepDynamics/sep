#include "core/logging.h"
#include <iostream>
#include <spdlog/spdlog.h>

namespace sep {
namespace logging {

void Manager::initialize() {
    // Stub implementation - just print to stderr
    std::cerr << "Logging initialized" << std::endl;
}

void Manager::shutdownLogging() {
    // Basic shutdown logic using spdlog
    spdlog::shutdown();
    std::cerr << "Logging shutdown" << std::endl;
}

} // namespace logging
} // namespace sep