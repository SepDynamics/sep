#include "core/manager.h"

namespace sep::config {

class ConfigManager::Impl {
public:
    MemoryThresholdConfig mem_cfg{};
    QuantumThresholdConfig quant_cfg{};
};

ConfigManager::ConfigManager() : impl_(std::make_unique<Impl>()) {}
ConfigManager::~ConfigManager() = default;

void ConfigManager::initialize(int, char**) {}

const SystemConfig &ConfigManager::getConfig() const {
    static SystemConfig cfg{}; return cfg;
}
void ConfigManager::setConfig(const SystemConfig &) {}

bool ConfigManager::loadFromFile(const sep::shim::string&) { return true; }
bool ConfigManager::loadFromEnvironment() { return true; }
bool ConfigManager::loadFromCommandLine(int, char**) { return true; }

const APIConfig &ConfigManager::getAPIConfig() const {
    static APIConfig cfg{}; return cfg;
}
const CudaConfig &ConfigManager::getCudaConfig() const {
    static CudaConfig cfg{}; return cfg;
}
const LogConfig &ConfigManager::getLogConfig() const {
    static LogConfig cfg{}; return cfg;
}
const MemoryThresholdConfig &ConfigManager::getMemoryConfig() const {
    return impl_->mem_cfg;
}
const QuantumThresholdConfig &ConfigManager::getQuantumConfig() const {
    return impl_->quant_cfg;
}
void ConfigManager::updateAPIConfig(const APIConfig &) {}
void ConfigManager::updateCudaConfig(const CudaConfig &) {}
void ConfigManager::updateLogConfig(const LogConfig &) {}
void ConfigManager::updateMemoryConfig(const MemoryThresholdConfig &cfg) { impl_->mem_cfg = cfg; }
void ConfigManager::updateQuantumConfig(const QuantumThresholdConfig &cfg) { impl_->quant_cfg = cfg; }
void ConfigManager::resetToDefaults() {
    impl_->mem_cfg = MemoryThresholdConfig{};
    impl_->quant_cfg = QuantumThresholdConfig{};
}

} // namespace sep::config
