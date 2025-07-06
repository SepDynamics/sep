#pragma once

// Math constants are already defined in crow.h
#ifdef CROW_DISABLE_RTTI
#include "crow/crow_isolation.h"
#endif

#include "api/rate_limit_middleware.h"
#include "api/types.h"
#include "api/auth_middleware.h"
#include "api/ollama_client.h"
#include "core/types.h"
#include <atomic>
#include <memory>
#include <mutex>
#include <string>

#include <nlohmann/json.hpp>
#include <thread>
#include <spdlog/spdlog.h>


// Forward declarations for Crow request/response and application
namespace crow {
class request;
class response;
template <typename... Middlewares>
class Crow;
}  // namespace crow
namespace sep {
namespace pattern { class PatternProcessor; }
namespace blender { namespace ccl { class CyclesRenderer; } }
}  // namespace sep
namespace sep::api {
class CrowRequest;

struct ServerMetrics {
  std::atomic<uint64_t> rateLimitedCount{0};
};

class Server {
 public:
  virtual ~Server() = default;

  virtual std::mutex& getMetricsMutex() = 0;
  virtual ServerMetrics& getModifiableMetrics() = 0;
  virtual std::unique_ptr<HttpResponse> makeJsonResponse(int code, const std::string& message) = 0;
  virtual std::string handleError(const std::string& message, int code) = 0;
  virtual void logRequest(const HttpRequest& req, int code, const std::string& body,
                          int64_t duration) = 0;
};


/**
 * @brief Concrete implementation of the SEP API Server
 *
 * This class implements the main HTTP API server for the SEP Engine,
 * providing REST endpoints for quantum processing, health checks, and metrics.
 */
class SEPApiServer : public Server {
 public:
  /**
   * @brief Construct a new SEPApiServer
   * @param config The API configuration
   * @param renderer Optional Cycles renderer dependency
   */
  SEPApiServer(const ::sep::config::APIConfig &config,
               sep::blender::ccl::CyclesRenderer *renderer);

  /**
   * @brief Destructor
   */
  ~SEPApiServer() override;

  // Delete copy constructor and assignment operator
  SEPApiServer(const SEPApiServer &) = delete;
  SEPApiServer &operator=(const SEPApiServer &) = delete;

  /**
   * @brief Start the server
   */
  void start();

  /**
   * @brief Register routes and start the server
   */
  bool run();

  /**
   * @brief Stop the server
   */
  void stop();

  /**
   * @brief Wait for server shutdown
   */
  void waitForShutdown();

  /**
   * @brief Update server configuration
   * @param new_config The new configuration
   */
  void updateConfig(const ::sep::config::APIConfig &new_config);

  // Server interface implementation
  std::mutex &getMetricsMutex() override;
  ServerMetrics &getModifiableMetrics() override;
  std::unique_ptr<HttpResponse> makeJsonResponse(int code,
                                                           const std::string &message) override;
  std::string handleError(const std::string &message, int code) override;
  void logRequest(const HttpRequest &req, int code, const std::string &body,
                  int64_t duration) override;

  /**
   * @brief Get error response string
   * @param message Error message
   * @param status HTTP status code
   * @return JSON error response string
   */
  std::string getErrorResponse(const std::string &message, int status);

 private:
  /**
   * @brief Make JSON response for Crow
   * @param status_code HTTP status code
   * @param data JSON data
   * @return Crow response
   */
  ::crow::response makeCrowJsonResponse(int status_code, const nlohmann::json &data);

  /**
   * @brief Modulate response detail based on coherence metrics (extract coherence from JSON)
   * @param response Original JSON response data
   * @return Possibly simplified JSON data
   */
  nlohmann::json applyCoherenceModulation(const nlohmann::json &response);

  /**
   * @brief Modulate response detail based on coherence metrics (explicit coherence score)
   * @param response Original JSON response data
   * @param coherence_score Explicit coherence score
   * @return Possibly simplified JSON data
   */
  nlohmann::json applyCoherenceModulation(const nlohmann::json &response, double coherence_score);

  /**
   * @brief Handle error and return JSON for Crow
   * @param message Error message
   * @param status_code HTTP status code
   * @return JSON error response
   */
  nlohmann::json handleCrowError(const std::string &message, int status_code);

  /**
   * @brief Log request details
   * @param req The request
   * @param status_code Response status code
   * @param response_body Response body
   * @param duration_ms Request duration in milliseconds
   */
  void logRequest(const ::crow::request &req, int status_code, const std::string &response_body,
                  int64_t duration_ms);

  /**
   * @brief Setup logging configuration
   */
  void setup_logging();

  /**
   * @brief Setup middleware
   */
  void setup_middleware();

  /**
   * @brief Setup API routes
   */
  void setup_routes();

  /**
   * @brief Setup Blender-specific routes
   */
  void setupBlenderRoutes();

  /**
   * @brief Setup signal handlers
   */
  void setup_signal_handlers();

  /**
   * @brief Initialize API clients
   */
  void initClients();

  /**
   * @brief Signal handler
   * @param signal Signal number
   */
  static void handleSignal(int signal);

  // Static instance for signal handling
  static SEPApiServer *instance_;

  // Configuration
  ::sep::config::APIConfig config_;

  // Logging
  std::shared_ptr<spdlog::logger> logger_;

  // Crow application
  std::unique_ptr<::crow::Crow<RateLimitMiddleware, AuthMiddleware>> app_;

  // Server thread
  std::unique_ptr<std::thread> server_thread_;

  // Running state
  std::atomic<bool> running_;

  // Metrics
  HealthMetrics metrics_;
  ServerMetrics server_metrics_;
  mutable std::mutex metrics_mutex_;

  // Clients
  std::unique_ptr<ollama::OllamaClient> ollama_client_;

  // Persistent processors for Blender routes
  std::unique_ptr<sep::pattern::PatternProcessor> pattern_processor_;
  sep::blender::ccl::CyclesRenderer *cycles_renderer_{nullptr};
};

}  // namespace sep::api
