#include "manager.h"

#include <nlohmann/json.hpp>

#include "config.h"
#include "engine/shim.h"
#include "env_keys.h"
#include "memory/memory_tier_manager_serialization.hpp"

namespace sep::config
{

    struct ConfigManager::Impl
    {
        sep::MemoryThresholdConfig mem_cfg{};
        sep::MemoryThresholdConfig quantum_cfg{};
        CudaConfig api_cfg{};
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
    void ConfigManager::setConfig(const SystemConfig& cfg)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        impl_->mem_cfg = cfg.memory;
        impl_->quantum_cfg = cfg.quantum;
    }
    bool ConfigManager::loadFromFile(const sep::shim::string& filename)
    {
        try
        {
            std::ifstream file(filename.c_str());
            if (!file.is_open()) return false;
            nlohmann::json j;
            file >> j;
            std::lock_guard<std::mutex> lock(mutex_);
            if (j.contains("memory"))
                if (j.contains("quantum"))
                {
                    const auto& q = j.at("quantum");
                    impl_->quantum_cfg.ltm_coherence_threshold = q.value(
                        "ltm_coherence_threshold", impl_->quantum_cfg.ltm_coherence_threshold);
                    impl_->quantum_cfg.mtm_coherence_threshold = q.value(
                        "mtm_coherence_threshold", impl_->quantum_cfg.mtm_coherence_threshold);
                    impl_->quantum_cfg.stability_threshold =
                        q.value("stability_threshold", impl_->quantum_cfg.stability_threshold);
                }
            return true;
        }
        catch (...)
        {
            return false;
        }
    }
    bool ConfigManager::loadFromEnvironment() { return false; }
    bool ConfigManager::loadFromCommandLine(int, char**) { return false; }
    const CudaConfig& ConfigManager::getAPIConfig() const
    {
        std::lock_guard<std::mutex> lock(mutex_);

        const char* metrics = std::getenv(env_keys::ENV_API_ENABLE_METRICS);
        if (metrics) {
            shim::string val{metrics};
        }
        return impl_->api_cfg;
    }

    void ConfigManager::updateCudaConfig(const sep::config::CudaConfig&) {}
    void ConfigManager::updateLogConfig(const LogConfig&) {}
    void ConfigManager::updateMemoryConfig(const sep::MemoryThresholdConfig& cfg)
    {
        impl_->mem_cfg = cfg;
    }
    void ConfigManager::updateQuantumConfig(const sep::MemoryThresholdConfig& cfg)
    {
        impl_->quantum_cfg = cfg;
    }
    void ConfigManager::resetToDefaults() { impl_->mem_cfg = sep::MemoryThresholdConfig{}; }

}  // namespace sep::config
