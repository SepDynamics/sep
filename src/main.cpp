#include "core/manager.h"
#include "core/engine.h"
#include "memory/manager.h"
#include "api/server.h"
#include <curl/curl.h>
#include <exception>
#include <iostream>
#include <spdlog/spdlog.h>
#include <string>
#include <csignal>

#ifndef SEP_HAS_EXCEPTIONS
#    if defined(__cpp_exceptions) || defined(__EXCEPTIONS) || defined(_CPPUNWIND)
#        define SEP_HAS_EXCEPTIONS 1
#    else
#        define SEP_HAS_EXCEPTIONS 0
#    endif
#endif

// Global flag for controlling server shutdown
volatile sig_atomic_t g_keep_running = 1;

// Signal handler for graceful shutdown
void signal_handler(int signal) {
    spdlog::info("Received signal {}, initiating shutdown...", signal);
    g_keep_running = 0;
}

int main(int argc, char* argv[]) {
  curl_global_init(CURL_GLOBAL_ALL);
  sep::logging::initializeLogging();

  // Setup signal handling for graceful shutdown
  signal(SIGINT, signal_handler);
  signal(SIGTERM, signal_handler);

  bool server_mode = false;
  for (int i = 1; i < argc; i++) {
    if (std::string(argv[i]) == "--server") {
      server_mode = true;
      break;
    }
  }

#if SEP_HAS_EXCEPTIONS
  try {
#endif
    auto& config = sep::config::ConfigManager::getInstance();
    config.initialize(argc, argv);

    sep::core::Engine engine;
    if (!engine.init(config.getAPIConfig())) {
#if SEP_HAS_EXCEPTIONS
      throw std::runtime_error("Engine initialization failed");
#else
      spdlog::critical("Engine initialization failed");
      curl_global_cleanup();
      sep::logging::shutdownLogging();
      return 1;
#endif
    }
    engine.run();

    sep::api::SEPApiServer server(config.getAPIConfig());
    server.run();
    
    if (server_mode) {
      spdlog::info("Running in server mode, waiting for shutdown signal...");
      // Keep running until signal is received
      while (g_keep_running) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
      }
      spdlog::info("Shutting down server...");
    } else {
      // In non-server mode, wait for server to complete
      server.waitForShutdown();
    }

    engine.shutdown();

#if SEP_HAS_EXCEPTIONS
  } catch (const std::exception& e) {
    spdlog::critical("Unhandled exception: {}", e.what());
    curl_global_cleanup();
    sep::logging::shutdownLogging();
    return 1;
  }
#endif

  curl_global_cleanup();
  sep::logging::shutdownLogging();
  return 0;
}
