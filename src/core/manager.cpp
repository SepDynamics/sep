#include "core/manager.h"

namespace sep::config {

struct ConfigManager::Impl {
    MemoryThresholdConfig mem_cfg{};
    QuantumThresholdConfig quantum_cfg{};
};

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
    return impl_->mem_cfg;
}
const QuantumThresholdConfig& ConfigManager::getQuantumConfig() const {
    return impl_->quantum_cfg;
}
void ConfigManager::updateAPIConfig(const APIConfig&) {}
void ConfigManager::updateCudaConfig(const CudaConfig&) {}
void ConfigManager::updateLogConfig(const LogConfig&) {}
void ConfigManager::updateMemoryConfig(const MemoryThresholdConfig& cfg) { impl_->mem_cfg = cfg; }
void ConfigManager::updateQuantumConfig(const QuantumThresholdConfig& cfg) { impl_->quantum_cfg = cfg; }
void ConfigManager::resetToDefaults() {
    impl_->mem_cfg = MemoryThresholdConfig{};
    impl_->quantum_cfg = QuantumThresholdConfig{};
}

} // namespace sep::config
