// Standard library includes
#include <chrono>
#include <csignal>
#include <cstdio>
#include <memory>
#include <string>
#include <thread>

// Compatibility layer includes
#include "compat/shim.h"

// Third-party includes
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

// Project includes
#include "core/logging.h"
#include "memory/memory_tier_manager.hpp"
#include "crow/crow_isolation.h"
#include "api/crow_request.h"
#include "api/json_helpers.h"
#include "api/types.h"
#include "api/ollama_client.h"
#include "core/types.h"
#include "core/common.h"
#include "api/rate_limit_middleware.h"
#include "api/request_interface.h"
#include "api/sep_engine.h"
#include "quantum/types.h"
#include "quantum/processor.h"
#include "api/server.h"
#include "quantum/cycles.h"
#include "compat/types.h"

#ifdef SEP_HAS_BLENDER
#include "blender/api.h"
#include "blender/cycles_renderer.h"
#endif

namespace sep::api {

#ifndef SEP_HAS_EXCEPTIONS
#    if defined(__cpp_exceptions) || defined(__EXCEPTIONS) || defined(_CPPUNWIND)
#        define SEP_HAS_EXCEPTIONS 1
#    else
#        define SEP_HAS_EXCEPTIONS 0
#    endif
#endif

// Static instance for signal handling
SEPApiServer* SEPApiServer::instance_ = nullptr;
SEPApiServer::SEPApiServer(const ::sep::config::APIConfig& config)
    : config_(config),
      logger_(nullptr),
      app_(nullptr),
      server_thread_(nullptr),
      running_(false),
      metrics_(),
      server_metrics_(),
      metrics_mutex_(),
      ollama_client_(nullptr),
      pattern_processor_(std::make_unique<sep::pattern::PatternProcessor>()),
      cycles_renderer_(std::make_unique<sep::blender::ccl::CyclesRenderer>()) {
    instance_ = this;

    // Initialize the Crow app with middlewares
    app_ = std::make_unique<::crow::Crow<RateLimitMiddleware, AuthMiddleware>>();

    // Initialize logger
    setup_logging();

    // Initialize Ollama client
    ollama_client_ = std::make_unique<ollama::OllamaClient>(config_.ollama);

    if (pattern_processor_) {
        (void)pattern_processor_->init(nullptr);
    }
#ifdef SEP_HAS_BLENDER
    if (cycles_renderer_) {
        (void)cycles_renderer_->initialize();
    }
#endif
}

SEPApiServer::~SEPApiServer() {
  if (running_.load()) {
    stop();
  }
  instance_ = nullptr;
}

std::mutex &SEPApiServer::getMetricsMutex() {
  return metrics_mutex_;
}

ServerMetrics &SEPApiServer::getModifiableMetrics() {
  return server_metrics_;
}

void SEPApiServer::start() {
    if (!app_) return;

    app_->port(config_.port);
    app_->multithreaded();

    setup_middleware();
    setup_routes();
}

bool SEPApiServer::run() {
  if (!app_) return false;

  // Ensure routes are registered just before starting
  setup_routes();
  start();

  // Start server in a new thread
  server_thread_ = std::make_unique<std::thread>([this]() {
    running_ = true;
    app_->run();
  });

  return true;
}


void SEPApiServer::stop() {
  if (!running_) {
    return;
  }

  logger_->info("Stopping SEP API Server");
  running_ = false;

  if (app_) {
    // Stop the Crow app first
    app_->stop();
    
    // Wait for server thread to finish
    if (server_thread_ && server_thread_->joinable()) {
      logger_->debug("Waiting for server thread to join...");
      server_thread_->join();
      server_thread_.reset();
    }
  }

  logger_->info("SEP API Server stopped");
}

void SEPApiServer::waitForShutdown() {
  if (server_thread_ && server_thread_->joinable()) {
    server_thread_->join();
  }
}

void SEPApiServer::updateConfig(const ::sep::config::APIConfig& new_config) {
  std::lock_guard<std::mutex> lock(metrics_mutex_);
  config_ = new_config;
  logger_->info("Configuration updated");
}


std::unique_ptr<HttpResponse> SEPApiServer::makeJsonResponse(int code, const std::string& message) {
  // This is a simplified implementation
  class SimpleHttpResponse : public HttpResponse {
   public:
    SimpleHttpResponse(int code, const std::string& body) : code_(code), body_(body) {}
    void setCode(int code) override { code_ = code; }
    void setBody(const std::string& body) override { body_ = body; }
    void end() override {}
    int getCode() const override { return code_; }
    std::string getBody() const override { return body_; }

   private:
    int code_;
    std::string body_;
  };

  nlohmann::json response;
  response["status"] = (code >= 200 && code < 300) ? "success" : "error";
  response["message"] = message;
  response["code"] = code;

  return std::make_unique<SimpleHttpResponse>(code, response.dump());
}

std::string SEPApiServer::handleError(const std::string& message, int code) {
  nlohmann::json error_response{};
  error_response["error"] = true;
  error_response["message"] = message;
  error_response["code"] = code;
  error_response["timestamp"] = std::chrono::duration_cast<std::chrono::seconds>(
                                    std::chrono::system_clock::now().time_since_epoch())
                                    .count();

  logger_->error("API Error [{}]: {}", code, message);

  std::lock_guard<std::mutex> lock(metrics_mutex_);
  metrics_.failedRequests++;
  metrics_.lastErrorCode = code;
  metrics_.lastErrorTime = std::chrono::system_clock::now();

  return error_response.dump();
}

void SEPApiServer::logRequest(const HttpRequest& req, int code, [[maybe_unused]] const std::string& response_body,
                               int64_t duration) {
    if (!logger_) return;
    std::lock_guard<std::mutex> lock(metrics_mutex_);

  metrics_.totalRequests++;
  if (code >= 200 && code < 300) {
    metrics_.successfulRequests++;
    metrics_.lastSuccessTime = std::chrono::system_clock::now();
  } else {
    metrics_.failedRequests++;
    metrics_.lastErrorCode = code;
    metrics_.lastErrorTime = std::chrono::system_clock::now();
  }

  // Update average response time
  double current_avg = metrics_.averageResponseTime.load();
  double new_avg = (current_avg * (metrics_.totalRequests - 1) + duration) / metrics_.totalRequests;
  metrics_.averageResponseTime = new_avg;

  metrics_.lastResponseTime = std::chrono::milliseconds(duration);

  logger_->info("Request: {} {} - Status: {} - Duration: {}ms",
                req.method(), req.url(), code, duration);
}

std::string SEPApiServer::getErrorResponse(const std::string& message, int status) {
  return handleError(message, status);
}


::crow::response SEPApiServer::makeCrowJsonResponse(int status_code, const nlohmann::json& data) {
  ::crow::response res(status_code);
  res.set_header("Content-Type", "application/json");
  res.body = data.dump();
  return res;
}

nlohmann::json SEPApiServer::handleCrowError(const std::string& message,
                                             int status_code) {
  nlohmann::json error_json{
      {"error", true},
      {"message", message},
      {"code", status_code},
      {"timestamp",
       std::chrono::duration_cast<std::chrono::seconds>(
           std::chrono::system_clock::now().time_since_epoch())
           .count()}};

  logger_->error("API Error [{}]: {}", status_code, message);

  std::lock_guard<std::mutex> lock(metrics_mutex_);
  metrics_.failedRequests++;
  metrics_.lastErrorCode = status_code;
  metrics_.lastErrorTime = std::chrono::system_clock::now();

  return error_json;
}

void SEPApiServer::logRequest(const ::crow::request& req, int status_code,
                               [[maybe_unused]] const std::string& response_body, int64_t duration_ms) {
    if (!logger_) return;
    std::lock_guard<std::mutex> lock(metrics_mutex_);

  metrics_.totalRequests++;
  if (status_code >= 200 && status_code < 300) {
    metrics_.successfulRequests++;
    metrics_.lastSuccessTime = std::chrono::system_clock::now();
  } else {
    metrics_.failedRequests++;
    metrics_.lastErrorCode = status_code;
    metrics_.lastErrorTime = std::chrono::system_clock::now();
  }

  // Update average response time
  double current_avg = metrics_.averageResponseTime.load();
  double new_avg =
      (current_avg * (metrics_.totalRequests - 1) + duration_ms) / metrics_.totalRequests;
  metrics_.averageResponseTime = new_avg;

  metrics_.lastResponseTime = std::chrono::milliseconds(duration_ms);

  std::string method_name = std::string(::crow::method_name(req.method));
  std::string url = std::string(req.url);

  logger_->info("Request: {} {} - Status: {} - Duration: {}ms",
                method_name, url, status_code, duration_ms);
}

void SEPApiServer::setup_logging() {
#if SEP_HAS_EXCEPTIONS
  try {
#endif
    ::sep::logging::Manager::initialize();
    logger_ = spdlog::get("sep_api");
    if (!logger_) {
      logger_ = spdlog::default_logger();
    }

    // Set log level based on config
    if (config_.log_level == "debug") {
      logger_->set_level(spdlog::level::debug);
    } else if (config_.log_level == "info") {
      logger_->set_level(spdlog::level::info);
    } else if (config_.log_level == "warn") {
      logger_->set_level(spdlog::level::warn);
    } else if (config_.log_level == "error") {
      logger_->set_level(spdlog::level::err);
    }

    logger_->info("Logging initialized for SEP API Server");
#if SEP_HAS_EXCEPTIONS
  } catch (const std::exception& e) {
    (void)fprintf(stderr,
                  "Failed to initialize logging: %s\n",
                  e.what());
  }
#endif
}

void SEPApiServer::setup_middleware() {
  if (!app_) return;

  // Configure rate limiting middleware
  auto& rate_limit_mw = app_->get_middleware<RateLimitMiddleware>();
  (void)rate_limit_mw; // Middleware is used implicitly through registration

  // Configure auth middleware
  auto& auth_mw = app_->get_middleware<AuthMiddleware>();
  // Set auth tokens if configured
  if (!config_.extra_settings.empty()) {
    auto tokens_it = config_.extra_settings.find("auth_tokens");
    if (tokens_it != config_.extra_settings.end()) {
      // Parse and set tokens - simplified implementation
      std::vector<std::string> tokens = {tokens_it->second};
      auth_mw.set_tokens(tokens);
    }
  }
}

void SEPApiServer::setup_routes() {
  if (!app_) return;

  auto& engine = SepEngine::getInstance();

  // Health check endpoint
  app_->route_dynamic("/api/v1/health")
      .methods(::crow::HTTPMethod::GET)([this, &engine](const ::crow::request& req) {
    auto start_time = std::chrono::steady_clock::now();

    #if SEP_HAS_EXCEPTIONS
    try {
#endif
      auto health_data = engine.getHealthStatus();
      auto end_time = std::chrono::steady_clock::now();
      auto duration =
          std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

      logRequest(req, HTTP_OK, health_data.dump(), duration);
      return makeCrowJsonResponse(HTTP_OK, health_data);

    #if SEP_HAS_EXCEPTIONS
    } catch (const std::exception& e) {
      
      auto end_time = std::chrono::steady_clock::now();
      auto duration =
          std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

      auto error_crow =
          handleCrowError("Health check failed: " + std::string(e.what()), HTTP_INTERNAL_ERROR);
      logRequest(req, HTTP_INTERNAL_ERROR, error_crow.dump(), duration);
      return makeCrowJsonResponse(HTTP_INTERNAL_ERROR, error_crow);
    }
#endif
  });

  // Process patterns endpoint
  app_->route_dynamic("/api/v1/pattern/evolve")
      .methods(::crow::HTTPMethod::POST)([this, &engine](const ::crow::request& req) {
        auto start_time = std::chrono::steady_clock::now();

#if SEP_HAS_EXCEPTIONS
        try {
#endif
          // Parse request body
          nlohmann::json request_data = parse_json(std::string(req.body));

          // Process patterns through SEP engine
          auto result = engine.processPatterns(request_data);
          auto response_data = applyCoherenceModulation(result);

          auto end_time = std::chrono::steady_clock::now();
          auto duration =
              std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

          logRequest(req, HTTP_OK, response_data.dump(), duration);
          return makeCrowJsonResponse(HTTP_OK, response_data);

        #if SEP_HAS_EXCEPTIONS
        } catch (const nlohmann::json::parse_error& e) {
          
          auto end_time = std::chrono::steady_clock::now();
          auto duration =
              std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

          auto error_crow =
              handleCrowError("Invalid JSON: " + std::string(e.what()), HTTP_BAD_REQUEST);
          logRequest(req, HTTP_BAD_REQUEST, error_crow.dump(), duration);
          return makeCrowJsonResponse(HTTP_BAD_REQUEST, error_crow);

        } catch (const std::exception& e) {
          
          auto end_time = std::chrono::steady_clock::now();
          auto duration =
              std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

          auto error_crow = handleCrowError("Pattern processing failed: " + std::string(e.what()),
                                            HTTP_INTERNAL_ERROR);
          logRequest(req, HTTP_INTERNAL_ERROR, error_crow.dump(), duration);
          return makeCrowJsonResponse(HTTP_INTERNAL_ERROR, error_crow);
        }
        #endif
      });

  // Process batch endpoint
  app_->route_dynamic("/api/v1/memory/query")
      .methods(::crow::HTTPMethod::POST)([this, &engine](const ::crow::request& req) {
        auto start_time = std::chrono::steady_clock::now();

#if SEP_HAS_EXCEPTIONS
        try {
          
#endif
          nlohmann::json request_data = parse_json(std::string(req.body));
          auto result = engine.processBatch(request_data);
          auto response_data = applyCoherenceModulation(result);

          auto end_time = std::chrono::steady_clock::now();
          auto duration =
              std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

          logRequest(req, HTTP_OK, response_data.dump(), duration);
          return makeCrowJsonResponse(HTTP_OK, response_data);

        #if SEP_HAS_EXCEPTIONS
        } catch (const nlohmann::json::parse_error& e) {
          
          auto end_time = std::chrono::steady_clock::now();
          auto duration =
              std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

          auto error_crow =
              handleCrowError("Invalid JSON: " + std::string(e.what()), HTTP_BAD_REQUEST);
          logRequest(req, HTTP_BAD_REQUEST, error_crow.dump(), duration);
          return makeCrowJsonResponse(HTTP_BAD_REQUEST, error_crow);

        } catch (const std::exception& e) {
          
          auto end_time = std::chrono::steady_clock::now();
          auto duration =
              std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

          auto error_crow = handleCrowError("Batch processing failed: " + std::string(e.what()),
                                            HTTP_INTERNAL_ERROR);
          logRequest(req, HTTP_INTERNAL_ERROR, error_crow.dump(), duration);
          return makeCrowJsonResponse(HTTP_INTERNAL_ERROR, error_crow);
        }
        #endif
      });

  // Pattern history endpoint
  app_->route_dynamic("/api/v1/patterns/history")
      .methods(::crow::HTTPMethod::POST)([this, &engine](const ::crow::request& req) {
        auto start_time = std::chrono::steady_clock::now();

#if SEP_HAS_EXCEPTIONS
        try {
          
#endif
          nlohmann::json request_data = parse_json(std::string(req.body));
          auto result = engine.getPatternHistory(request_data);
          auto response_data = applyCoherenceModulation(result);

          auto end_time = std::chrono::steady_clock::now();
          auto duration =
              std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

          logRequest(req, HTTP_OK, response_data.dump(), duration);
          return makeCrowJsonResponse(HTTP_OK, response_data);

        #if SEP_HAS_EXCEPTIONS
        } catch (const nlohmann::json::parse_error& e) {
          
          auto end_time = std::chrono::steady_clock::now();
          auto duration =
              std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

          auto error_crow =
              handleCrowError("Invalid JSON: " + std::string(e.what()), HTTP_BAD_REQUEST);
          logRequest(req, HTTP_BAD_REQUEST, error_crow.dump(), duration);
          return makeCrowJsonResponse(HTTP_BAD_REQUEST, error_crow);

        } catch (const std::exception& e) {
          
          auto end_time = std::chrono::steady_clock::now();
          auto duration =
              std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

          auto error_crow = handleCrowError("Pattern history failed: " + std::string(e.what()),
                                            HTTP_INTERNAL_ERROR);
          logRequest(req, HTTP_INTERNAL_ERROR, error_crow.dump(), duration);
          return makeCrowJsonResponse(HTTP_INTERNAL_ERROR, error_crow);
        }
        #endif
      });

  // Validate contexts endpoint
  app_->route_dynamic("/api/v1/context/process")
      .methods(::crow::HTTPMethod::POST)([this, &engine](const ::crow::request& req) {
        auto start_time = std::chrono::steady_clock::now();

#if SEP_HAS_EXCEPTIONS
        try {
          
#endif
          nlohmann::json request_data = parse_json(std::string(req.body));
          auto result = engine.validateContexts(request_data);
          auto response_data = applyCoherenceModulation(result);

          auto end_time = std::chrono::steady_clock::now();
          auto duration =
              std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

          logRequest(req, HTTP_OK, response_data.dump(), duration);
          return makeCrowJsonResponse(HTTP_OK, response_data);

        #if SEP_HAS_EXCEPTIONS
        } catch (const nlohmann::json::parse_error& e) {
          
          auto end_time = std::chrono::steady_clock::now();
          auto duration =
              std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

          auto error_crow =
              handleCrowError("Invalid JSON: " + std::string(e.what()), HTTP_BAD_REQUEST);
          logRequest(req, HTTP_BAD_REQUEST, error_crow.dump(), duration);
          return makeCrowJsonResponse(HTTP_BAD_REQUEST, error_crow);

        } catch (const std::exception& e) {
          
          auto end_time = std::chrono::steady_clock::now();
          auto duration =
              std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

          auto error_crow = handleCrowError("Context validation failed: " + std::string(e.what()),
                                            HTTP_INTERNAL_ERROR);
          logRequest(req, HTTP_INTERNAL_ERROR, error_crow.dump(), duration);
          return makeCrowJsonResponse(HTTP_INTERNAL_ERROR, error_crow);
        }
        #endif
      });

  // Extract embeddings endpoint
  app_->route_dynamic("/api/v1/embeddings/extract")
      .methods(::crow::HTTPMethod::POST)([this, &engine](const ::crow::request& req) {
        auto start_time = std::chrono::steady_clock::now();

#if SEP_HAS_EXCEPTIONS
        try {
          
#endif
          nlohmann::json request_data = parse_json(std::string(req.body));
          auto result = engine.extractEmbeddings(request_data);
          auto response_data = applyCoherenceModulation(result);

          auto end_time = std::chrono::steady_clock::now();
          auto duration =
              std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

          logRequest(req, HTTP_OK, response_data.dump(), duration);
          return makeCrowJsonResponse(HTTP_OK, response_data);

        #if SEP_HAS_EXCEPTIONS
        } catch (const nlohmann::json::parse_error& e) {
          
          auto end_time = std::chrono::steady_clock::now();
          auto duration =
              std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

          auto error_crow =
              handleCrowError("Invalid JSON: " + std::string(e.what()), HTTP_BAD_REQUEST);
          logRequest(req, HTTP_BAD_REQUEST, error_crow.dump(), duration);
          return makeCrowJsonResponse(HTTP_BAD_REQUEST, error_crow);

        } catch (const std::exception& e) {
          
          auto end_time = std::chrono::steady_clock::now();
          auto duration =
              std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

          auto error_crow = handleCrowError("Embedding extraction failed: " + std::string(e.what()),
                                            HTTP_INTERNAL_ERROR);
          logRequest(req, HTTP_INTERNAL_ERROR, error_crow.dump(), duration);
          return makeCrowJsonResponse(HTTP_INTERNAL_ERROR, error_crow);
        }
        #endif
      });

  // Analyze patterns endpoint
  app_->route_dynamic("/api/v1/pattern/analyze")
      .methods(::crow::HTTPMethod::POST)([this, &engine](const ::crow::request& req) {
        auto start_time = std::chrono::steady_clock::now();

#if SEP_HAS_EXCEPTIONS
        try {
          
#endif
          nlohmann::json request_data = parse_json(std::string(req.body));
          auto result = engine.calculateSimilarity(request_data);
          auto response_data = applyCoherenceModulation(result);

          auto end_time = std::chrono::steady_clock::now();
          auto duration =
              std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

          logRequest(req, HTTP_OK, response_data.dump(), duration);
          return makeCrowJsonResponse(HTTP_OK, response_data);

        #if SEP_HAS_EXCEPTIONS
        } catch (const nlohmann::json::parse_error& e) {
          
          auto end_time = std::chrono::steady_clock::now();
          auto duration =
              std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

          auto error_crow =
              handleCrowError("Invalid JSON: " + std::string(e.what()), HTTP_BAD_REQUEST);
          logRequest(req, HTTP_BAD_REQUEST, error_crow.dump(), duration);
          return makeCrowJsonResponse(HTTP_BAD_REQUEST, error_crow);

        } catch (const std::exception& e) {
          
          auto end_time = std::chrono::steady_clock::now();
          auto duration =
              std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

          auto error_crow = handleCrowError(
              "Similarity calculation failed: " + std::string(e.what()), HTTP_INTERNAL_ERROR);
          logRequest(req, HTTP_INTERNAL_ERROR, error_crow.dump(), duration);
          return makeCrowJsonResponse(HTTP_INTERNAL_ERROR, error_crow);
        }
        #endif
      });

  // Context relationships endpoint
  app_->route_dynamic("/api/v1/context/relationships")
      .methods(::crow::HTTPMethod::POST)([this, &engine](const ::crow::request& req) {
        auto start_time = std::chrono::steady_clock::now();

#if SEP_HAS_EXCEPTIONS
        try {
          
#endif
          nlohmann::json request_data = parse_json(std::string(req.body));
          auto result = engine.blendContexts(request_data);
          auto response_data = applyCoherenceModulation(result);

          auto end_time = std::chrono::steady_clock::now();
          auto duration =
              std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

          logRequest(req, HTTP_OK, response_data.dump(), duration);
          return makeCrowJsonResponse(HTTP_OK, response_data);

        #if SEP_HAS_EXCEPTIONS
        } catch (const nlohmann::json::parse_error& e) {
          
          auto end_time = std::chrono::steady_clock::now();
          auto duration =
              std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

          auto error_crow =
              handleCrowError("Invalid JSON: " + std::string(e.what()), HTTP_BAD_REQUEST);
          logRequest(req, HTTP_BAD_REQUEST, error_crow.dump(), duration);
          return makeCrowJsonResponse(HTTP_BAD_REQUEST, error_crow);

        } catch (const std::exception& e) {
          
          auto end_time = std::chrono::steady_clock::now();
          auto duration =
              std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

          auto error_crow = handleCrowError("Context blending failed: " + std::string(e.what()),
                                            HTTP_INTERNAL_ERROR);
          logRequest(req, HTTP_INTERNAL_ERROR, error_crow.dump(), duration);
          return makeCrowJsonResponse(HTTP_INTERNAL_ERROR, error_crow);
        }
        #endif
      });

  // Memory metrics endpoint
  app_->route_dynamic("/api/v1/metrics/memory")
      .methods(::crow::HTTPMethod::GET)([this, &engine](const ::crow::request& req) {
        auto start_time = std::chrono::steady_clock::now();

#if SEP_HAS_EXCEPTIONS
        try {
          
#endif
          auto result = engine.getMemoryMetrics();
          auto response_data = applyCoherenceModulation(result);

          auto end_time = std::chrono::steady_clock::now();
          auto duration =
              std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

          logRequest(req, HTTP_OK, response_data.dump(), duration);
          return makeCrowJsonResponse(HTTP_OK, response_data);

        #if SEP_HAS_EXCEPTIONS
        } catch (const std::exception& e) {
          
          auto end_time = std::chrono::steady_clock::now();
          auto duration =
              std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

          auto error_crow = handleCrowError("Memory metrics failed: " + std::string(e.what()),
                                            HTTP_INTERNAL_ERROR);

          logRequest(req, HTTP_INTERNAL_ERROR, error_crow.dump(), duration);
          return makeCrowJsonResponse(HTTP_INTERNAL_ERROR, error_crow);
        }
        #endif
      });

  logger_->info("API routes configured successfully");
}

nlohmann::json SEPApiServer::applyCoherenceModulation(const nlohmann::json& response, double coherence_score) {
  if (!config_.response_modulation.enabled) {
    return response;
  }

  nlohmann::json modulated_response = response;

  // Apply coherence-based response modulation
  if (coherence_score < config_.response_modulation.coherence_threshold) {
    if (config_.response_modulation.simplify_low_coherence) {
      // Simplify response for low coherence
      if (modulated_response.contains("details") && modulated_response["details"].is_array()) {
        auto& details = modulated_response["details"];
        if (details.size() > static_cast<size_t>(config_.response_modulation.max_detail_level)) {
          details = nlohmann::json::array();
          for (size_t i = 0; i < static_cast<size_t>(config_.response_modulation.max_detail_level); ++i) {
            if (i < response["details"].size()) {
              details.push_back(response["details"][i]);
            }
          }
        }
      }

      // Add coherence warning for low scores
      modulated_response["coherence_warning"] = "Response simplified due to low coherence score";
      modulated_response["coherence_score"] = coherence_score;
    }
  }

  // Add coherence metadata
  modulated_response["modulation"] = {
    {"applied", true},
    {"coherence_score", coherence_score},
    {"threshold", config_.response_modulation.coherence_threshold},
    {"simplified", coherence_score < config_.response_modulation.coherence_threshold}
  };

  logger_->debug("Applied coherence modulation: score={}, threshold={}",
                 coherence_score, config_.response_modulation.coherence_threshold);

  return modulated_response;
}

nlohmann::json SEPApiServer::applyCoherenceModulation(const nlohmann::json& response) {
  double coherence_score = 0.0;

  if (response.contains("coherence_score") && response["coherence_score"].is_number()) {
    coherence_score = response["coherence_score"].get<double>();
  } else if (response.contains("metrics") && response["metrics"].is_object() &&
             response["metrics"].contains("coherence_score") &&
             response["metrics"]["coherence_score"].is_number()) {
    coherence_score = response["metrics"]["coherence_score"].get<double>();
  } else {
    logger_->debug("No coherence_score found; returning original response");
    return response;
  }

  coherence_score = std::clamp(coherence_score, 0.0, 1.0);
  logger_->debug("Extracted coherence score: {} from response", coherence_score);
  return applyCoherenceModulation(response, coherence_score);
}

void SEPApiServer::setup_signal_handlers() {
  (void)std::signal(SIGINT, handleSignal);
  (void)std::signal(SIGTERM, handleSignal);
}

void SEPApiServer::initClients() {
#if SEP_HAS_EXCEPTIONS
  try {
#endif
    // Initialize Ollama client if configured
    ollama_client_ = std::make_unique<ollama::OllamaClient>(config_.ollama);
    logger_->info("Ollama client initialized");
#if SEP_HAS_EXCEPTIONS
  } catch (const std::exception& e) {
    logger_->warn("Failed to initialize Ollama client: {}", e.what());
  }
#endif
}

void SEPApiServer::handleSignal(int signal) {
  if (instance_) {
    instance_->logger_->info("Received signal {}, shutting down", signal); 
    instance_->stop();
  }
}

void SEPApiServer::setupBlenderRoutes() {
#ifdef SEP_HAS_BLENDER
    // Pattern processing endpoint
    app_->route_dynamic("/api/v1/patterns/process")
    .methods(::crow::HTTPMethod::POST)([this](const ::crow::request& req) {
        try {
            auto json = nlohmann::json::parse(std::string(req.body));
            if (!json.is_object()) {
                ::crow::response res;
                res.code = HTTP_BAD_REQUEST;
                res.write("Invalid JSON format");
                return res;
            }

            // Extract mesh data
            auto mesh = json["mesh"];
            auto config = json["config"];
            
            // Configure processing
            sep::quantum::ProcessingConfig proc_config;
            proc_config.max_patterns = config.value("max_patterns", 10000);
            proc_config.mutation_rate = config.value("mutation_rate", 0.01f);
            proc_config.ltm_coherence_threshold = config.value("ltm_threshold", 0.9f);
            proc_config.mtm_coherence_threshold = config.value("mtm_threshold", 0.6f);
            proc_config.stability_threshold = config.value("stability", 0.8f);
            proc_config.enable_cuda = config.value("enable_cuda", false);
            
            // Use persistent pattern processor
            auto* processor = pattern_processor_.get();
            if (!processor) {
                ::crow::response res;
                res.body = "Pattern processor unavailable";
                res.code = 500;
                return res;
            }

            // Process vertices through quantum manifold
            std::vector<sep::pattern::PatternData> patterns;
            for (const auto& v : mesh["vertices"]) {
                sep::pattern::PatternData pattern;
                pattern.position = glm::vec4(
                    v[0].get<float>(),
                    v[1].get<float>(),
                    v[2].get<float>(),
                    1.0f
                );
                pattern.attributes = glm::vec4(0.0f);
                patterns.push_back(pattern);
            }

            // Add patterns to processor
            for (const auto& pattern : patterns) {
                if (processor->addPattern(pattern) != sep::SEPResult::SUCCESS) {
                    ::crow::response res;
                    res.body = "Failed to add pattern to processor";
                    res.code = 500;
                    return res;
                }
            }

            // Process patterns
            const auto& results = processor->process();

            // Prepare response JSON
            nlohmann::json response;
            response["success"] = true;

            // Processed patterns
            nlohmann::json processed_patterns = nlohmann::json::array();
            for (const auto& pattern : results) {
                nlohmann::json processed;
                nlohmann::json attr_array = nlohmann::json::array();
                attr_array.push_back(pattern.attributes.x);
                attr_array.push_back(pattern.attributes.y);
                attr_array.push_back(pattern.attributes.z);
                attr_array.push_back(pattern.attributes.w);
                processed["attributes"] = attr_array;
                processed["metrics"] = {
                    {"coherence", pattern.coherence},
                    {"stability", pattern.stability},
                    {"entropy", pattern.entropy}
                };
                processed_patterns.push_back(processed);
            }
            response["processed_patterns"] = processed_patterns;

            // Return success response
            ::crow::response res;
            res.body = response.dump();
            res.code = 200;
            return res;

        } catch (const std::exception& e) {
            ::crow::response res;
            res.body = std::string("Error processing request: ") + e.what();
            res.code = 500;
            return res;
        }
    });

    // Cycles evolution endpoint
    app_->route_dynamic("/api/v1/cycles/evolve")
    .methods(::crow::HTTPMethod::POST)([this](const ::crow::request& req) {
        try {
            auto json = nlohmann::json::parse(std::string(req.body));
            
            // Get parameters
            std::string object_name = json["object_name"].get<std::string>();
            int iterations = json["iterations"].get<int>();
            auto pattern_state = json["pattern_state"];
            auto cycles_config = json["cycles_config"];

            // Use persistent Cycles renderer
            auto* renderer = cycles_renderer_.get();
            if (!renderer) {
                ::crow::response res;
                res.body = "Cycles renderer unavailable";
                res.code = 500;
                return res;
            }
            renderer->initialize();

            // Build pattern data from state
            sep::pattern::PatternData pattern;
            pattern.coherence = pattern_state.value("coherence", 0.5f);
            pattern.stability = pattern_state.value("stability", 0.5f);
            pattern.entropy = pattern_state.value("entropy", 0.5f);

            std::vector<sep::pattern::PatternData> patterns{pattern};

            if (renderer->createSceneFromPatterns(patterns) != sep::SEPResult::SUCCESS) {
                ::crow::response res;
                res.body = "Failed to create scene";
                res.code = 500;
                return res;
            }

            sep::blender::ccl::CyclesRenderer::RenderParams params;
            params.width = cycles_config.value("width", 640);
            params.height = cycles_config.value("height", 480);
            params.samples = cycles_config.value("samples", 32);
            params.output_format = cycles_config.value("output", std::string("render.ppm"));

            if (renderer->renderScene(params) != sep::SEPResult::SUCCESS) {
                ::crow::response res;
                res.body = "Render failed";
                res.code = 500;
                return res;
            }

            nlohmann::json response;
            response["success"] = true;
            
            ::crow::response res;
            res.body = response.dump();
            res.code = 200;
            return res;
            
        } catch (const std::exception& e) {
            ::crow::response res;
            res.body = std::string("Error processing request: ") + e.what();
            res.code = 500;
            return res;
        }
    });
    
    // Health check endpoint
    app_->route_dynamic("/health")
    .methods(::crow::HTTPMethod::GET)
    ([](const ::crow::request&) {
        nlohmann::json response = {
            {"status", "running"},
            {"engine_initialized", sep::api::SepEngine::getInstance().getHealthStatus()["initialized"]},
            {"quantum_processor_ready", true},
            {"cycles_available", true}, // Cycles support determined at build time
            {"timestamp", std::time(nullptr)}
        };
        
        ::crow::response res;
        res.body = response.dump();
        res.code = 200;
        return res;
        
    });
#endif  // SEP_HAS_BLENDER

}


} // end namespace sep::api
