#include "core/manager.h"
#include "core/env_keys.h"
#include "compat/shim.h"

namespace sep::config {

namespace {
using namespace env_keys;

class ConfigManager::Impl {
public:
  Impl() {
    loadDefaults();
    loadFromEnvironment();
  }

  void loadDefaults() { config = SystemConfig{}; }

  bool loadFromEnvironment() {
    bool configLoaded = false;

    // Try to load from config file path in environment
    const char *env_path = std::getenv(ENV_CONFIG_PATH);
    if (env_path) {
      configLoaded = loadFromFile(env_path);
    }

    // Load individual environment variables
    if (const char *port = std::getenv(ENV_API_PORT)) {
      config.api.port = std::stoi(port);
    }
    if (const char *threads = std::getenv(ENV_API_THREADS)) {
      config.api.threads = std::stoi(threads);
    }
    if (const char *enable = std::getenv(ENV_API_ENABLE_METRICS)) {
      sep::shim::string val = enable;
      config.api.enable_metrics = (val == "1" || val == "true");
    }
    if (const char *keep_alive = std::getenv(ENV_API_KEEP_ALIVE)) {
      config.api.keep_alive_timeout_ms = std::stoul(keep_alive);
    }
    if (const char *log_level = std::getenv(ENV_LOG_LEVEL)) {
      config.logging.level = log_level;
    }
    if (const char *log_dir = std::getenv(ENV_LOG_DIR)) {
      config.logging.log_dir = log_dir;
    }
    if (const char *log_file = std::getenv(ENV_LOG_FILE)) {
      config.logging.log_file = log_file;
    }
    if (const char *console_output = std::getenv(ENV_LOG_CONSOLE)) {
      sep::shim::string val = console_output;
      config.logging.console_output = (val == "1" || val == "true");
    }
    if (const char *data_dir = std::getenv(ENV_DATA_DIR)) {
      config.data_path = data_dir;
    }

    return configLoaded;
  }

  bool loadFromFile(const sep::shim::string &filename) {
    std::ifstream file(filename.c_str());
    if (!file.is_open()) {
      return false;
    }

      std::stringstream buffer;
      buffer << file.rdbuf();

      auto json = nlohmann::json::parse(buffer.str(), nullptr, false);
      if (json.is_discarded()) {
        return false;
      }

      // Parse API config using direct conversion
      if (json.contains("api")) {
        json.at("api").get_to(config.api);
      }

      // Parse logging config
      if (json.contains("logging")) {
        const auto &logging = json["logging"];
        config.logging.level = logging.value("level", "info");
        config.logging.log_dir = logging.value("log_dir", "logs");
        config.logging.log_file = logging.value("log_file", "sep.log");
        config.logging.console_output = logging.value("console_output", true);
        config.logging.file_output = logging.value("file_output", true);
      }

      return true;
  }

  bool loadFromCommandLine(int argc, char *argv[]) {
    for (int i = 1; i < argc; ++i) {
      sep::shim::string arg = argv[i];

      // Support both --key=value and --key value
      sep::shim::string value;
      auto pos = arg.find('=');
      if (pos != sep::shim::string::npos) {
        // Extract substring after '='
        std::string arg_str = arg.c_str();
        value = arg_str.substr(pos + 1).c_str();
        arg = arg_str.substr(0, pos).c_str();
      } else if (i + 1 < argc) {
        value = argv[++i];
      } else {
        continue;
      }

      if (arg == "--config") {
        if (!loadFromFile(value)) {
          return false;
        }
      } else if (arg == "--port") {
        config.api.port = std::stoi(value);
      } else if (arg == "--threads") {
        config.api.threads = std::stoi(value);
      } else if (arg == "--log-level") {
        config.api.log_level = value;
      } else if (arg == "--enable-metrics") {
        config.api.enable_metrics = (value == "1" || value == "true");
      } else if (arg == "--keep-alive-timeout-ms") {
        config.api.keep_alive_timeout_ms = std::stoul(value);
      }
    }
    return true;
  }

  SystemConfig config;
};
} // anonymous namespace

ConfigManager::ConfigManager() : impl_(std::make_unique<Impl>()) {}
ConfigManager::~ConfigManager() {}

const SystemConfig &ConfigManager::getConfig() const {
  std::lock_guard<std::mutex> lock(mutex_);
  return impl_->config;
}

void ConfigManager::setConfig(const SystemConfig &config) {
  std::lock_guard<std::mutex> lock(mutex_);
  impl_->config = config;
}

bool ConfigManager::loadFromFile(const sep::shim::string &filename) {
  std::lock_guard<std::mutex> lock(mutex_);
  return impl_->loadFromFile(filename);
}

bool ConfigManager::loadFromEnvironment() {
  std::lock_guard<std::mutex> lock(mutex_);
  return impl_->loadFromEnvironment();
}

bool ConfigManager::loadFromCommandLine(int argc, char *argv[]) {
  std::lock_guard<std::mutex> lock(mutex_);
  return impl_->loadFromCommandLine(argc, argv);
}

const APIConfig &ConfigManager::getAPIConfig() const {
  std::lock_guard<std::mutex> lock(mutex_);
  return impl_->config.api;
}

const CUDAConfig &ConfigManager::getCUDAConfig() const {
  std::lock_guard<std::mutex> lock(mutex_);
  return impl_->config.cuda;
}

const LogConfig &ConfigManager::getLogConfig() const {
  std::lock_guard<std::mutex> lock(mutex_);
  return impl_->config.logging;
}

void ConfigManager::updateAPIConfig(const APIConfig &config) {
  std::lock_guard<std::mutex> lock(mutex_);
  impl_->config.api = config;
}

void ConfigManager::updateCUDAConfig(const CUDAConfig &config) {
  std::lock_guard<std::mutex> lock(mutex_);
  impl_->config.cuda = config;
}

void ConfigManager::updateLogConfig(const LogConfig &config) {
  std::lock_guard<std::mutex> lock(mutex_);
  impl_->config.logging = config;
}

void ConfigManager::resetToDefaults() {
  std::lock_guard<std::mutex> lock(mutex_);
  impl_->loadDefaults();
}

void ConfigManager::initialize(int argc, char **argv) {
  std::lock_guard<std::mutex> lock(mutex_);
  impl_->loadDefaults();
  impl_->loadFromEnvironment();
  impl_->loadFromCommandLine(argc, argv);
}

} // namespace sep::config
