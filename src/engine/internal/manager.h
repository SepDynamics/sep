#ifndef SEP_CONFIG_MANAGER_H
#define SEP_CONFIG_MANAGER_H

#include "config.h"
#include "engine/internal/standard_includes.h"
#include "memory/memory_tier_manager.hpp"
#include "types.h"

namespace sep::config {

class ConfigManager {
public:
  static ConfigManager &getInstance() {
    static ConfigManager instance;
    return instance;
  }

  // Delete copy operations
  ConfigManager(const ConfigManager &) = delete;
  ConfigManager &operator=(const ConfigManager &) = delete;

  void initialize(int argc, char *argv[]);

  // Configuration access
  const SystemConfig &getConfig() const;
  void setConfig(const SystemConfig &config);

  bool loadFromFile(const std::string &filename);
  bool loadFromEnvironment();
  bool loadFromCommandLine(int argc, char *argv[]);

  // Access individual config sections
  const SystemConfig &getAPIConfig() const;
  void updateAPIConfig(const SystemConfig &config);

  void updateCudaConfig(const sep::config::CudaConfig &config);
  void updateLogConfig(const LogConfig &config);
  void updateMemoryConfig(const sep::memory::MemoryThresholdConfig &config);
  void updateQuantumConfig(const sep::QuantumThresholdConfig &config);

  // Reset configuration to defaults
  void resetToDefaults();

public:
  ~ConfigManager();

protected:
  ConfigManager();

private:
  struct Impl;
  std::unique_ptr<Impl> impl_;
  mutable std::mutex mutex_;
};

} // namespace sep::config

#endif // SEP_CONFIG_MANAGER_H
