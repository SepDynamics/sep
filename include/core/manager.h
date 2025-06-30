#ifndef SEP_CONFIG_MANAGER_H
#define SEP_CONFIG_MANAGER_H

#include "types.h"
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

  // Initialize configuration. Sources are merged in the following order:
  // defaults < file < environment < command line.
  void initialize(int argc, char *argv[]);

  // Configuration access
  const SystemConfig &getConfig() const;
  void setConfig(const SystemConfig &config);

  // Load configuration
  // Order of precedence:
  //   1. Defaults compiled into the application or loaded from file.
  //   2. Environment variables overriding defaults.
  //   3. Command line arguments overriding both.
  bool loadFromFile(const sep::shim::string &filename);
  bool loadFromEnvironment();
  bool loadFromCommandLine(int argc, char *argv[]);

  // Component-specific access
  const APIConfig &getAPIConfig() const;
  const CudaConfig &getCudaConfig() const;
  const LogConfig &getLogConfig() const;
  const MemoryConfig &getMemoryConfig() const;
  const QuantumConfig &getQuantumConfig() const;

  // Update specific components
  void updateAPIConfig(const APIConfig &config);
  void updateCudaConfig(const CudaConfig &config);
  void updateLogConfig(const LogConfig &config);
  void updateMemoryConfig(const MemoryConfig &config);
  void updateQuantumConfig(const QuantumConfig &config);

  // Reset configuration to defaults
  void resetToDefaults();

public:
  ~ConfigManager();

protected:
  ConfigManager();

private:
  class Impl;
  std::unique_ptr<Impl> impl_;
  mutable std::mutex mutex_;
};

} // namespace sep::config

#endif // SEP_CONFIG_MANAGER_H
