#include "core/manager.h"

#include <fstream>
#include <nlohmann/json.hpp>

#include "memory/memory_tier_manager_serialization.hpp"

namespace sep::config
{

    struct ConfigManager::Impl
    {
        MemoryThresholdConfig mem_cfg{};
#if SEP_BUILD_QUANTUM
        QuantumThresholdConfig quantum_cfg{};
#endif
        bool loaded{false};
    };

    ConfigManager::ConfigManager() : impl_(std::make_unique<Impl>()) {}
    ConfigManager::~ConfigManager() = default;

    void ConfigManager::initialize(int, char**)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (!impl_->loaded)
        {
            loadFromFile("config.json");
            impl_->loaded = true;
        }
    }
    const SystemConfig& ConfigManager::getConfig() const
    {
        static SystemConfig cfg{};
        return cfg;
    }
    void ConfigManager::setConfig(const SystemConfig&) {}
    bool ConfigManager::loadFromFile(const sep::shim::string& filename)
    {
        try
        {
            std::ifstream file(filename.c_str());
            if (!file.is_open()) return false;
            nlohmann::json j;
            file >> j;
            std::lock_guard<std::mutex> lock(mutex_);
            if (j.contains("memory")) impl_->mem_cfg = j.at("memory").get<MemoryThresholdConfig>();
#if SEP_BUILD_QUANTUM
            if (j.contains("quantum"))
            {
                const auto& q = j.at("quantum");
                impl_->quantum_cfg.ltm_coherence_threshold =
                    q.value("ltm_coherence_threshold", impl_->quantum_cfg.ltm_coherence_threshold);
                impl_->quantum_cfg.mtm_coherence_threshold =
                    q.value("mtm_coherence_threshold", impl_->quantum_cfg.mtm_coherence_threshold);
                impl_->quantum_cfg.stability_threshold =
                    q.value("stability_threshold", impl_->quantum_cfg.stability_threshold);
            }
#endif
            return true;
        }
        catch (...)
        {
            return false;
        }
    }
    bool ConfigManager::loadFromEnvironment() { return false; }
    bool ConfigManager::loadFromCommandLine(int, char**) { return false; }
    const APIConfig& ConfigManager::getAPIConfig() const
    {
        static APIConfig cfg{};
        return cfg;
    }
    const CudaConfig& ConfigManager::getCudaConfig() const
    {
        static CudaConfig cfg{};
        return cfg;
    }
    const LogConfig& ConfigManager::getLogConfig() const
    {
        static LogConfig cfg{};
        return cfg;
    }
    const MemoryThresholdConfig& ConfigManager::getMemoryConfig() const { return impl_->mem_cfg; }
    const QuantumThresholdConfig& ConfigManager::getQuantumConfig() const
    {
#if SEP_BUILD_QUANTUM
        return impl_->quantum_cfg;
#else
        static QuantumThresholdConfig cfg{};
        return cfg;
#endif
    }
    void ConfigManager::updateAPIConfig(const APIConfig&) {}
    void ConfigManager::updateCudaConfig(const CudaConfig&) {}
    void ConfigManager::updateLogConfig(const LogConfig&) {}
    void ConfigManager::updateMemoryConfig(const MemoryThresholdConfig& cfg)
    {
        impl_->mem_cfg = cfg;
    }
    void ConfigManager::updateQuantumConfig(const QuantumThresholdConfig& cfg)
    {
#if SEP_BUILD_QUANTUM
        impl_->quantum_cfg = cfg;
#else
        (void)cfg;
#endif
    }
    void ConfigManager::resetToDefaults()
    {
        impl_->mem_cfg = MemoryThresholdConfig{};
#if SEP_BUILD_QUANTUM
        impl_->quantum_cfg = QuantumThresholdConfig{};
#endif
    }

}  // namespace sep::config
