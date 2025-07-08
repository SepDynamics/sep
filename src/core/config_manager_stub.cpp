#include "core/manager.h"

namespace sep::config {

class ConfigManager::Impl {
public:
    MemoryThresholdConfig mem_cfg{};
#if SEP_BUILD_QUANTUM
    QuantumThresholdConfig quantum_cfg{};
#endif
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
    static QuantumThresholdConfig cfg{};
#if SEP_BUILD_QUANTUM
    return impl_->quantum_cfg;
#else
    return cfg;
#endif
}
void ConfigManager::updateAPIConfig(const APIConfig &) {}
void ConfigManager::updateCudaConfig(const CudaConfig &) {}
void ConfigManager::updateLogConfig(const LogConfig &) {}
void ConfigManager::updateMemoryConfig(const MemoryThresholdConfig &cfg) { impl_->mem_cfg = cfg; }
void ConfigManager::updateQuantumConfig(const QuantumThresholdConfig &cfg) {
#if SEP_BUILD_QUANTUM
    impl_->quantum_cfg = cfg;
#else
    (void)cfg;
#endif
}

void ConfigManager::resetToDefaults() {
    impl_->mem_cfg = MemoryThresholdConfig{};
#if SEP_BUILD_QUANTUM
    impl_->quantum_cfg = QuantumThresholdConfig{};
#endif
}

} // namespace sep::config
