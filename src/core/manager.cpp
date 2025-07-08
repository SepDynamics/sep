#include "core/manager.h"

namespace sep::config {

struct ConfigManager::Impl {};

ConfigManager::ConfigManager() = default;
ConfigManager::~ConfigManager() = default;

void ConfigManager::initialize(int, char**) {}
const SystemConfig& ConfigManager::getConfig() const {
    static SystemConfig cfg{};
    return cfg;
}
void ConfigManager::setConfig(const SystemConfig&) {}
bool ConfigManager::loadFromFile(const sep::shim::string&) { return false; }
bool ConfigManager::loadFromEnvironment() { return false; }
bool ConfigManager::loadFromCommandLine(int, char**) { return false; }
const APIConfig& ConfigManager::getAPIConfig() const {
    static APIConfig cfg{};
    return cfg;
}
const CudaConfig& ConfigManager::getCudaConfig() const {
    static CudaConfig cfg{};
    return cfg;
}
const LogConfig& ConfigManager::getLogConfig() const {
    static LogConfig cfg{};
    return cfg;
}
const MemoryThresholdConfig& ConfigManager::getMemoryConfig() const {
    static MemoryThresholdConfig cfg{};
    return cfg;
}
const QuantumThresholdConfig& ConfigManager::getQuantumConfig() const {
    static QuantumThresholdConfig cfg{};
    return cfg;
}
void ConfigManager::updateAPIConfig(const APIConfig&) {}
void ConfigManager::updateCudaConfig(const CudaConfig&) {}
void ConfigManager::updateLogConfig(const LogConfig&) {}
void ConfigManager::updateMemoryConfig(const MemoryThresholdConfig&) {}
void ConfigManager::updateQuantumConfig(const QuantumThresholdConfig&) {}
void ConfigManager::resetToDefaults() {}

} // namespace sep::config
