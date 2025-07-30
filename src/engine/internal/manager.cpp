#include "manager.h"

#include <nlohmann/json.hpp>

#include "config.h"
#include "engine/internal/standard_includes.h"
#include "env_keys.h"
#include "memory/memory_tier_manager_serialization.hpp"
#include "memory/types.h"

namespace sep::config
{

    struct ConfigManager::Impl
    {
        sep::memory::MemoryThresholdConfig mem_cfg{};
        sep::QuantumThresholdConfig quantum_cfg{};
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
    bool ConfigManager::loadFromFile(const std::string& filename)
    {
        try
        {
            std::ifstream file(filename.c_str());
            if (!file.is_open()) return false;
            nlohmann::json j;
            file >> j;
            std::lock_guard<std::mutex> lock(mutex_);
            if (j.contains("memory"))
            {
                const auto& m = j.at("memory");
                impl_->mem_cfg.stm_size = m.value("stm_size", impl_->mem_cfg.stm_size);
                impl_->mem_cfg.mtm_size = m.value("mtm_size", impl_->mem_cfg.mtm_size);
                impl_->mem_cfg.ltm_size = m.value("ltm_size", impl_->mem_cfg.ltm_size);
                impl_->mem_cfg.promote_stm_to_mtm =
                    m.value("promote_stm_to_mtm", impl_->mem_cfg.promote_stm_to_mtm);
                impl_->mem_cfg.promote_mtm_to_ltm =
                    m.value("promote_mtm_to_ltm", impl_->mem_cfg.promote_mtm_to_ltm);
                impl_->mem_cfg.demote_threshold =
                    m.value("demote_threshold", impl_->mem_cfg.demote_threshold);
                impl_->mem_cfg.fragmentation_threshold =
                    m.value("fragmentation_threshold", impl_->mem_cfg.fragmentation_threshold);
                impl_->mem_cfg.use_unified_memory =
                    m.value("use_unified_memory", impl_->mem_cfg.use_unified_memory);
                impl_->mem_cfg.enable_compression =
                    m.value("enable_compression", impl_->mem_cfg.enable_compression);
                impl_->mem_cfg.stm_to_mtm_min_gen =
                    m.value("stm_to_mtm_min_gen", impl_->mem_cfg.stm_to_mtm_min_gen);
                impl_->mem_cfg.mtm_to_ltm_min_gen =
                    m.value("mtm_to_ltm_min_gen", impl_->mem_cfg.mtm_to_ltm_min_gen);
            }
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
            return true;
        }
        catch (...)
        {
            return false;
        }
    }
    bool ConfigManager::loadFromEnvironment() { return false; }
    bool ConfigManager::loadFromCommandLine(int, char**) { return false; }
    const SystemConfig& ConfigManager::getAPIConfig() const
    {
        std::lock_guard<std::mutex> lock(mutex_);

        const char* metrics = std::getenv(env_keys::ENV_API_ENABLE_METRICS);
        if (metrics) {
            std::string val{metrics};
        }
        static SystemConfig cfg{};
        return cfg;
    }

    void ConfigManager::updateCudaConfig(const sep::config::CudaConfig&) {}
    void ConfigManager::updateLogConfig(const LogConfig&) {}
    void ConfigManager::updateMemoryConfig(const sep::memory::MemoryThresholdConfig& cfg)
    {
        impl_->mem_cfg = cfg;
    }
    void ConfigManager::updateQuantumConfig(const sep::QuantumThresholdConfig& cfg)
    {
        impl_->quantum_cfg = cfg;
    }
    void ConfigManager::resetToDefaults() { impl_->mem_cfg = sep::memory::MemoryThresholdConfig{}; }

}  // namespace sep::config
