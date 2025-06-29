#ifndef SEP_CONFIG_TYPES_H
#define SEP_CONFIG_TYPES_H

// Include order optimized for CUDA compatibility
#ifdef __CUDACC__
#include "compat/cuda_common.h"
#include "compat/cuda_helpers.h"
#endif

// Project headers (must come before std headers for proper isolation)
#include "compat/cuda.h"
#include "api/types.h"
#include "api/ollama_types.h"

// Standard C headers
#include <cstddef>
#include <cstdint>

// Standard C++ headers
#include <map>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

namespace sep {
namespace config {

// Default configuration values
constexpr const char* DEFAULT_LOG_LEVEL = "info";
constexpr const char* DEFAULT_LOG_FILE  = "sep.log";
constexpr const char* DEFAULT_LOG_DIR   = "logs";

#ifdef __CUDACC__
#    define SEP_CUDA_ATTR SEP_HOST SEP_DEVICE
#else
#    define SEP_CUDA_ATTR
#endif

struct CORSConfig
{
    SEP_CUDA_ATTR            CORSConfig() : enabled(false), origins("*") {}
    bool                     enabled;
    std::string              origins;
    std::vector<std::string> allowed_headers;
};

struct RateLimitConfig
{
    bool     enabled{true};
    uint32_t rpm{60};
};

struct LoggingConfig
{
    bool        log_requests{true};
    bool        log_responses{false};
    std::string log_file{"api.log"};
    size_t      max_file_size{10 * 1024 * 1024};  // 10MB
    size_t      max_files{5};
    std::string level{"info"};
};

struct APIConfig
{
    std::size_t                        max_connections{1000};
    std::size_t                        timeout_ms{5000};
    std::string                        host{"127.0.0.1"};
    std::uint16_t                      port{8080};
    std::uint32_t                      threads{4};
    std::size_t                        keep_alive_timeout_ms{15000};
    std::string                        log_level{"info"};
    bool                               enable_metrics{true};
    std::size_t                        max_batch_size{1024};
    CORSConfig                         cors;
    RateLimitConfig                    rate_limit;
    LoggingConfig                      logging;
    ::sep::ollama::OllamaConfig          ollama;
    std::map<std::string, std::string> extra_settings;

    struct ResponseModulationConfig
    {
        bool   enabled{true};
        double coherence_threshold{0.7};
        bool   simplify_low_coherence{true};
        size_t max_detail_level{3};
    } response_modulation;
};

struct CUDAConfig
{
    bool        use_gpu{true};
    std::size_t max_memory_mb{8192};  // 8GB
    std::size_t batch_size{1024};
    float       gpu_memory_limit{0.9f};
    bool        enable_profiling{false};
};

struct LogConfig
{
    std::string level{"info"};                            // Log level (debug, info, warn, error)
    std::string log_dir{"logs"};                          // Directory for log files
    std::string log_file{"sep.log"};                      // Main log file name
    bool        console_output{true};                     // Enable console logging
    bool        file_output{true};                        // Enable file logging
    bool        json_format{false};                       // Use JSON format for logs
    std::size_t max_file_size{1024 * 1024 * 10};          // 10MB
    std::size_t max_files{5};                             // Maximum number of rotated files
    std::size_t flush_interval{1};                        // Flush interval in seconds
    std::string pattern{"%Y-%m-%d %H:%M:%S.%e [%l] %v"};  // Log pattern
    bool        async{true};                              // Use async logging
    std::size_t queue_size{8192};                         // Async queue size
};

struct SemanticConfig {
    int embedding_dimensions{512};
    double coherence_threshold{0.7};
    bool enable_adaptive_processing{true};
    std::size_t max_batch_size{1024};
};

struct AnalyticsConfig {
    bool enable_profiling{false};
    bool track_memory_usage{true};
    bool collect_metrics{true};
    std::size_t history_size{1000};
    double sampling_rate{0.1};
};

struct SystemConfig {
    APIConfig   api;
    CUDAConfig  cuda;
    LogConfig   logging;
    std::string data_path;
};

// JSON serialization helpers
inline void to_json(nlohmann::json& j, const CORSConfig& c)
{
    j = nlohmann::json{{"enabled", c.enabled}, {"allowed_origins", c.origins}};
    if (!c.allowed_headers.empty())
    {
        j["allowed_headers"] = c.allowed_headers;
    }
}

inline void from_json(const nlohmann::json& j, CORSConfig& c)
{
    c.enabled = j.value("enabled", false);
    c.origins = j.value("allowed_origins", "*");
    if (j.contains("allowed_headers"))
    {
        c.allowed_headers = j["allowed_headers"].get<std::vector<std::string>>();
    }
}

inline void to_json(nlohmann::json& j, const RateLimitConfig& r)
{
    j = nlohmann::json{{"enabled", r.enabled}, {"rpm", r.rpm}};
}

inline void from_json(const nlohmann::json& j, RateLimitConfig& r)
{
    r.enabled = j.value("enabled", true);
    r.rpm     = j.value("rpm", 60u);
}

inline void to_json(nlohmann::json& j, const LoggingConfig& l)
{
    j = nlohmann::json{{"log_requests", l.log_requests},
                       {"log_responses", l.log_responses},
                       {"log_file", l.log_file},
                       {"max_file_size", l.max_file_size},
                       {"max_files", l.max_files},
                       {"level", l.level}};
}

inline void from_json(const nlohmann::json& j, LoggingConfig& l)
{
    l.log_requests  = j.value("log_requests", true);
    l.log_responses = j.value("log_responses", false);
    l.log_file      = j.value("log_file", std::string{"api.log"});
    l.max_file_size = j.value("max_file_size", static_cast<size_t>(10 * 1024 * 1024));
    l.max_files     = j.value("max_files", static_cast<size_t>(5));
    l.level         = j.value("level", std::string{"info"});
}

inline void to_json(nlohmann::json& j, const APIConfig& c)
{
    j = nlohmann::json{{"max_connections", c.max_connections},
                       {"timeout_ms", c.timeout_ms},
                       {"host", c.host},
                       {"port", c.port},
                       {"threads", c.threads},
                       {"keep_alive_timeout_ms", c.keep_alive_timeout_ms},
                       {"log_level", c.log_level},
                       {"enable_metrics", c.enable_metrics},
                       {"max_batch_size", c.max_batch_size},
                       {"cors", c.cors},
                       {"rate_limit", c.rate_limit},
                       {"logging", c.logging},
                       {"ollama", {{"host", c.ollama.host}, {"model", c.ollama.model}}},
                       {"extra_settings", c.extra_settings},
                       {"response_modulation",
                        {{"enabled", c.response_modulation.enabled},
                         {"coherence_threshold", c.response_modulation.coherence_threshold},
                         {"simplify_low_coherence", c.response_modulation.simplify_low_coherence},
                         {"max_detail_level", c.response_modulation.max_detail_level}}}};
}

inline void from_json(const nlohmann::json& j, APIConfig& c)
{
    c.max_connections       = j.value("max_connections", static_cast<size_t>(1000));
    c.timeout_ms            = j.value("timeout_ms", static_cast<size_t>(5000));
    c.host                  = j.value("host", std::string{"127.0.0.1"});
    c.port                  = j.value("port", static_cast<uint16_t>(8080));
    c.threads               = j.value("threads", static_cast<uint32_t>(4));
    c.keep_alive_timeout_ms = j.value("keep_alive_timeout_ms", static_cast<size_t>(15000));
    c.log_level             = j.value("log_level", std::string{"info"});
    c.enable_metrics        = j.value("enable_metrics", true);
    c.max_batch_size        = j.value("max_batch_size", static_cast<size_t>(1024));
    if (j.contains("cors"))
        j.at("cors").get_to(c.cors);
    if (j.contains("rate_limit"))
        j.at("rate_limit").get_to(c.rate_limit);
    if (j.contains("logging"))
        j.at("logging").get_to(c.logging);
    if (j.contains("ollama"))
    {
        c.ollama.host  = j["ollama"].value("host", c.ollama.host);
        c.ollama.model = j["ollama"].value("model", c.ollama.model);
    }
    if (j.contains("extra_settings"))
    {
        c.extra_settings = j["extra_settings"].get<std::map<std::string, std::string>>();
    }
    if (j.contains("response_modulation"))
    {
        const auto& rm                               = j["response_modulation"];
        c.response_modulation.enabled                = rm.value("enabled", true);
        c.response_modulation.coherence_threshold    = rm.value("coherence_threshold", 0.7);
        c.response_modulation.simplify_low_coherence = rm.value("simplify_low_coherence", true);
        c.response_modulation.max_detail_level       = rm.value("max_detail_level", static_cast<size_t>(3));
    }
}

// JSON serialization helpers for quantum configs
inline void to_json(nlohmann::json& j, const SemanticConfig& c) {
    j = nlohmann::json{
        {"embedding_dimensions", c.embedding_dimensions},
        {"coherence_threshold", c.coherence_threshold},
        {"enable_adaptive_processing", c.enable_adaptive_processing},
        {"max_batch_size", c.max_batch_size}
    };
}

inline void from_json(const nlohmann::json& j, SemanticConfig& c) {
    c.embedding_dimensions = j.value("embedding_dimensions", 512);
    c.coherence_threshold = j.value("coherence_threshold", 0.7);
    c.enable_adaptive_processing = j.value("enable_adaptive_processing", true);
    c.max_batch_size = j.value("max_batch_size", static_cast<size_t>(1024));
}

inline void to_json(nlohmann::json& j, const AnalyticsConfig& c) {
    j = nlohmann::json{
        {"enable_profiling", c.enable_profiling},
        {"track_memory_usage", c.track_memory_usage},
        {"collect_metrics", c.collect_metrics},
        {"history_size", c.history_size},
        {"sampling_rate", c.sampling_rate}
    };
}

inline void from_json(const nlohmann::json& j, AnalyticsConfig& c) {
    c.enable_profiling = j.value("enable_profiling", false);
    c.track_memory_usage = j.value("track_memory_usage", true);
    c.collect_metrics = j.value("collect_metrics", true);
    c.history_size = j.value("history_size", static_cast<size_t>(1000));
    c.sampling_rate = j.value("sampling_rate", 0.1);
}

}  // namespace config

enum class PatternStateEnum {
    UNINITIALIZED = 0,
    INITIALIZING,
    ACTIVE,
    STOPPED,
    ERROR
};

enum class StreamFlags { Default = 0, NonBlocking = 1 };
}  // namespace sep

#endif  // SEP_CONFIG_TYPES_H
