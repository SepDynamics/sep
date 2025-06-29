#include "core/logging.h"
#include <iostream>

namespace sep {
namespace logging {

void Manager::initialize() {
    // Stub implementation - just print to stderr
    std::cerr << "Logging initialized" << std::endl;
}

void Manager::shutdownLogging() {
    // Stub implementation - just print to stderr
    std::cerr << "Logging shutdown" << std::endl;
}

} // namespace logging
} // namespace sep