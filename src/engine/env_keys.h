#ifndef SEP_CONFIG_ENV_KEYS_H
#define SEP_CONFIG_ENV_KEYS_H

namespace sep {
namespace config {
namespace env_keys {

constexpr const char *ENV_CONFIG_PATH = "SEP_CONFIG_PATH";
constexpr const char *ENV_LOG_DIR = "SEP_LOG_DIR";
constexpr const char *ENV_LOG_LEVEL = "SEP_LOG_LEVEL";
constexpr const char *ENV_LOG_FILE = "SEP_LOG_FILE";
constexpr const char *ENV_LOG_CONSOLE = "SEP_LOG_CONSOLE";
constexpr const char *ENV_API_PORT = "SEP_API_PORT";
constexpr const char *ENV_API_THREADS = "SEP_API_THREADS";
constexpr const char *ENV_API_ENABLE_METRICS = "SEP_API_ENABLE_METRICS";
constexpr const char *ENV_API_KEEP_ALIVE = "SEP_API_KEEP_ALIVE_TIMEOUT_MS";
constexpr const char *ENV_DATA_DIR = "SEP_DATA_DIR";

} // namespace env_keys
} // namespace config
} // namespace sep

#endif // SEP_CONFIG_ENV_KEYS_H
