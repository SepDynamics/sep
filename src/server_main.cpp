#include "api/server.h"
#include "core/manager.h"
#include "core/logging.h"
#ifdef SEP_HAS_CYCLES
#include "blender/cycles_renderer.h"
#endif
#include <iostream>
#include <memory>

int main(int argc, char** argv) {
    sep::logging::Manager::initialize();
    auto logger = sep::logging::Manager::getInstance().getLogger("main");
    logger->info("SEP Engine starting up...");

    auto& cfg_manager = sep::config::ConfigManager::getInstance();
    cfg_manager.initialize(argc, argv);
    const auto& api_cfg = cfg_manager.getAPIConfig();
    logger->info("Configuration loaded. API will run on port {}.", api_cfg.port);

#ifdef SEP_HAS_CYCLES
    // Only create the renderer when Cycles is enabled
    std::unique_ptr<sep::blender::ccl::CyclesRenderer> renderer = nullptr;
    renderer = std::make_unique<sep::blender::ccl::CyclesRenderer>();
    if (renderer->initialize() != sep::SEPResult::SUCCESS) {
        logger->critical("Failed to initialize Cycles Renderer!");
        return 1;
    }
    
    // Pass the renderer to the server when Cycles is enabled
    sep::api::SEPApiServer server(api_cfg, renderer.get());
#else
    // Pass nullptr when Cycles is disabled
    sep::api::SEPApiServer server(api_cfg, nullptr);
#endif

    logger->info("Server object created.");

    if (!server.run()) {
        logger->critical("Failed to run the server!");
        return 1;
    }
    logger->info("Server is running. Waiting for shutdown signal...");
    server.waitForShutdown();
    logger->info("SEP Engine shutting down cleanly.");
    return 0;
}
