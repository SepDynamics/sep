#include "core/logging.h"
#include <iostream>
#include <spdlog/spdlog.h>

namespace sep {
namespace logging {

// Manager::initialize() is defined in src/memory/manager.cpp

void Manager::shutdownLogging() {
    // Basic shutdown logic using spdlog
    spdlog::shutdown();
    std::cerr << "Logging shutdown" << std::endl;
}

} // namespace logging
} // namespace sep