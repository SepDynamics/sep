#ifndef SEP_CONFIG_MANAGER_H
#define SEP_CONFIG_MANAGER_H

#include "memory/memory_tier_manager.hpp"
#include "core/types.h"
#include "core/config.h"
#include <memory>
#include <mutex>
#include <string>
#include "compat/shim.h"

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
  const workbench::SystemConfig &getConfig() const;
  void setConfig(const workbench::SystemConfig &config);

  bool loadFromFile(const sep::shim::string &filename);
  bool loadFromEnvironment();
  bool loadFromCommandLine(int argc, char *argv[]);

  // Access individual config sections
  const APIConfig &getAPIConfig() const;
  void updateAPIConfig(const APIConfig &config);

  void updateCudaConfig(const sep::config::CudaConfig &config);
  void updateLogConfig(const workbench::LogConfig &config);
  void updateMemoryConfig(const workbench::MemoryThresholdConfig &config);
  void updateQuantumConfig(const workbench::QuantumThresholdConfig &config);

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
