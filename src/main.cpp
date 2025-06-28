#include "core/manager.h"
#include "core/engine.h"
#include "memory/memory_tier_manager.hpp"
#include "api/server.h"
#include "blender/cycles_renderer.h"
#include <curl/curl.h>
#include <exception>
#include <iostream>
#include <fstream>
#include <spdlog/spdlog.h>
#include <string>
#include <csignal>
#include <nlohmann/json.hpp>

#ifndef SEP_HAS_EXCEPTIONS
#    if defined(__cpp_exceptions) || defined(__EXCEPTIONS) || defined(_CPPUNWIND)
#        define SEP_HAS_EXCEPTIONS 1
#    else
#        define SEP_HAS_EXCEPTIONS 0
#    endif
#endif

// Global flag for controlling server shutdown
std::atomic<int> g_keep_running{1};

// Signal handler for graceful shutdown
void signal_handler(int signal) {
    spdlog::info("Received signal {}, initiating shutdown...", signal);
    g_keep_running.store(0);
}

int main(int argc, char* argv[]) {
  curl_global_init(CURL_GLOBAL_ALL);
  sep::logging::Manager::initialize();

  // Setup signal handling for graceful shutdown
  signal(SIGINT, signal_handler);
  signal(SIGTERM, signal_handler);

  bool server_mode = false;
  bool cycles_mode = false;
  std::string render_file;
  
  for (int i = 1; i < argc; i++) {
    std::string arg = argv[i];
    if (arg == "--server") {
      server_mode = true;
    }
    else if (arg == "--cycles") {
      cycles_mode = true;
    }
    else if (arg == "--render" && i + 1 < argc) {
      render_file = argv[++i];
    }
  }

#if SEP_HAS_EXCEPTIONS
  try {
#endif
    auto& config = sep::config::ConfigManager::getInstance();
    config.initialize(argc, argv);

    // Handle Cycles rendering if requested
    if (cycles_mode && !render_file.empty()) {
      spdlog::info("Initializing Cycles renderer...");
      
#if SEP_HAS_CYCLES
      // Create the Cycles renderer
      sep::blender::CyclesRenderer renderer;
      
      // Initialize the renderer
      sep::SEPResult result = renderer.initialize();
      if (result != sep::SEPResult::SUCCESS) {
        spdlog::critical("Failed to initialize Cycles renderer");
        curl_global_cleanup();
        sep::logging::shutdownLogging();
        return 1;
      }
      
      spdlog::info("Loading scene from {}", render_file);
      
      // Load scene from JSON file
      std::ifstream file(render_file);
      if (!file.is_open()) {
        spdlog::critical("Failed to open scene file: {}", render_file);
        curl_global_cleanup();
        sep::logging::shutdownLogging();
        return 1;
      }
      
      // Parse JSON and convert to pattern data
      std::string json_content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
      file.close();
      
      try {
        auto json = nlohmann::json::parse(json_content);
        std::vector<sep::pattern::PatternData> patterns;
        
        // Convert JSON to pattern data
        if (json.contains("patterns") && json["patterns"].is_array()) {
          for (const auto& item : json["patterns"]) {
            sep::pattern::PatternData pattern;
            pattern.coherence = item.value("coherence", 0.5f);
            pattern.stability = item.value("stability", 0.5f);
            pattern.entropy = item.value("entropy", 0.5f);
            patterns.push_back(pattern);
          }
        } else if (json.is_array()) {
          // For backward compatibility with array-only format
          for (const auto& item : json) {
            sep::pattern::PatternData pattern;
            pattern.coherence = item.value("coherence", 0.5f);
            pattern.stability = item.value("stability", 0.5f);
            pattern.entropy = item.value("entropy", 0.5f);
            patterns.push_back(pattern);
          }
        }
        
        // Create scene from patterns
        result = renderer.createSceneFromPatterns(patterns);
        if (result != sep::SEPResult::SUCCESS) {
          spdlog::critical("Failed to create scene from patterns");
          curl_global_cleanup();
          sep::logging::shutdownLogging();
          return 1;
        }
        
        // Set up render parameters
        sep::blender::CyclesRenderer::RenderParams params;
        params.width = json.value("width", 1920);
        params.height = json.value("height", 1080);
        params.samples = json.value("samples", 128);
        params.output_path = json.value("output", "render.ppm");
        
        // Render the scene
        spdlog::info("Rendering scene to {}", params.output_path);
        result = renderer.renderScene(params);
        if (result != sep::SEPResult::SUCCESS) {
          spdlog::critical("Failed to render scene");
          curl_global_cleanup();
          sep::logging::shutdownLogging();
          return 1;
        }
        
        spdlog::info("Render completed successfully");
        curl_global_cleanup();
        sep::logging::shutdownLogging();
        return 0;
      }
      catch (const std::exception& e) {
        spdlog::critical("Error parsing scene file: {}", e.what());
        curl_global_cleanup();
        sep::logging::shutdownLogging();
        return 1;
      }
#else
      // Even though Cycles support is not available in this build,
      // we can still use the stub implementation
      spdlog::info("Using Cycles stub implementation");
      
      // Create the Cycles renderer (will use stub implementation)
      sep::blender::CyclesRenderer renderer;
      
      // Initialize the renderer
      sep::SEPResult result = renderer.initialize();
      if (result != sep::SEPResult::SUCCESS) {
        spdlog::critical("Failed to initialize Cycles renderer");
        curl_global_cleanup();
        sep::logging::shutdownLogging();
        return 1;
      }
      
      spdlog::info("Loading scene from {}", render_file);
      
      // Load scene from JSON file
      std::ifstream file(render_file);
      if (!file.is_open()) {
        spdlog::critical("Failed to open scene file: {}", render_file);
        curl_global_cleanup();
        sep::logging::shutdownLogging();
        return 1;
      }
      
      // Parse JSON and convert to pattern data
      std::string json_content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
      file.close();
      
      try {
        auto json = nlohmann::json::parse(json_content);
        std::vector<sep::pattern::PatternData> patterns;
        
        // Convert JSON to pattern data
        if (json.contains("patterns") && json["patterns"].is_array()) {
          for (const auto& item : json["patterns"]) {
            sep::pattern::PatternData pattern;
            pattern.coherence = item.value("coherence", 0.5f);
            pattern.stability = item.value("stability", 0.5f);
            pattern.entropy = item.value("entropy", 0.5f);
            patterns.push_back(pattern);
          }
        } else if (json.is_array()) {
          // For backward compatibility with array-only format
          for (const auto& item : json) {
            sep::pattern::PatternData pattern;
            pattern.coherence = item.value("coherence", 0.5f);
            pattern.stability = item.value("stability", 0.5f);
            pattern.entropy = item.value("entropy", 0.5f);
            patterns.push_back(pattern);
          }
        }
        
        // Create scene from patterns
        result = renderer.createSceneFromPatterns(patterns);
        if (result != sep::SEPResult::SUCCESS) {
          spdlog::critical("Failed to create scene from patterns");
          curl_global_cleanup();
          sep::logging::shutdownLogging();
          return 1;
        }
        
        // Set up render parameters
        sep::blender::CyclesRenderer::RenderParams params;
        params.width = json.value("width", 1920);
        params.height = json.value("height", 1080);
        params.samples = json.value("samples", 128);
        params.output_path = json.value("output", "render.ppm");
        
        // Render the scene
        spdlog::info("Rendering scene to {}", params.output_path);
        result = renderer.renderScene(params);
        if (result != sep::SEPResult::SUCCESS) {
          spdlog::critical("Failed to render scene");
          curl_global_cleanup();
          sep::logging::shutdownLogging();
          return 1;
        }
        
        spdlog::info("Render completed successfully");
        curl_global_cleanup();
        sep::logging::shutdownLogging();
        return 0;
      }
      catch (const std::exception& e) {
        spdlog::critical("Error parsing scene file: {}", e.what());
        curl_global_cleanup();
        sep::logging::shutdownLogging();
        return 1;
      }
#endif
    }

    // Normal engine initialization and server mode
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
      while (g_keep_running.load()) { // Fix: Use load() on atomic variable
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
