#pragma once

namespace sep {
namespace logging {

class Manager {
public:
    static void initialize();
    static void shutdownLogging();
};

} // namespace logging
} // namespace sep