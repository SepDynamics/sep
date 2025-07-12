#include "core/manager.h"
#include "core/config.h"

#include <fstream>
#include <nlohmann/json.hpp>
#include <cstdlib>
#include "core/env_keys.h"

#include "memory/memory_tier_manager_serialization.hpp"

namespace sep::config
{

    struct ConfigManager::Impl
    {
        workbench::MemoryThresholdConfig mem_cfg{};
#if SEP_BUILD_QUANTUM
        QuantumThresholdConfig quantum_cfg{};
#endif
        APIConfig api_cfg{};
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
#if SEP_BUILD_QUANTUM
        impl_->quantum_cfg = cfg.quantum;
#else
        (void)cfg;
#endif
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
                impl_->mem_cfg = j.at("memory").get<workbench::MemoryThresholdConfig>();
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
        std::lock_guard<std::mutex> lock(mutex_);

        const char* port = std::getenv(env_keys::ENV_API_PORT);
        if (port) impl_->api_cfg.port = static_cast<uint16_t>(std::atoi(port));

        const char* threads = std::getenv(env_keys::ENV_API_THREADS);
        if (threads) impl_->api_cfg.threads = static_cast<uint32_t>(std::atoi(threads));

        const char* metrics = std::getenv(env_keys::ENV_API_ENABLE_METRICS);
        if (metrics) {
            std::string val{metrics};
            impl_->api_cfg.enable_metrics = !(val == "0" || val == "false" || val == "False");
        }

        const char* log_level = std::getenv(env_keys::ENV_LOG_LEVEL);
        if (log_level) impl_->api_cfg.log_level = log_level;

        return impl_->api_cfg;
    }
    const sep::config::CudaConfig& ConfigManager::getCudaConfig() const
    {
        static sep::config::CudaConfig cfg{};
        return cfg;
    }
    const LogConfig& ConfigManager::getLogConfig() const
    {
        static LogConfig cfg{};
        return cfg;
    }
    const workbench::MemoryThresholdConfig& ConfigManager::getMemoryConfig() const { return impl_->mem_cfg; }
    const QuantumThresholdConfig& ConfigManager::getQuantumConfig() const
    {
#if SEP_BUILD_QUANTUM
        return impl_->quantum_cfg;
#else
        static QuantumThresholdConfig cfg{};
        return cfg;
#endif
    }
    void ConfigManager::updateAPIConfig(const APIConfig& cfg)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        impl_->api_cfg = cfg;
    }
    void ConfigManager::updateCudaConfig(const sep::config::CudaConfig&) {}
    void ConfigManager::updateLogConfig(const LogConfig&) {}
    void ConfigManager::updateMemoryConfig(const workbench::MemoryThresholdConfig& cfg)
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
        impl_->mem_cfg = workbench::MemoryThresholdConfig{};
#if SEP_BUILD_QUANTUM
        impl_->quantum_cfg = QuantumThresholdConfig{};
#endif
    }

}  // namespace sep::config
