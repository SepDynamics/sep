#pragma once
#include <string>

namespace sep::logging {

class Logger {
public:
    void log(const std::string& msg) {}
};

class Manager {
public:
    static Logger& getLogger(const std::string& name);
};

} // namespace sep::logging