#include "sep/logging/manager.hpp"

namespace sep::logging {

Logger& Manager::getLogger(const std::string& name) {
    static Logger logger;
    return logger;
}

} // namespace sep::logging