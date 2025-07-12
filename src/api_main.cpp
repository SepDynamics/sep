#include "core/manager.h"
#include "sep_engine_wrapper.h"
#include "api/server.h"
#include <memory>
#include <iostream>

int main(int argc, char** argv) {
    auto& configMgr = sep::config::ConfigManager::getInstance();
    configMgr.initialize(argc, argv);
    const auto& apiConfig = configMgr.getAPIConfig();

    // Create minimal renderer implementation
    std::unique_ptr<sep::CyclesRenderer> renderer = sep::createRenderer();
    if (renderer) {
        renderer->initialize();
    }

    sep::api::SEPApiServer server(apiConfig, static_cast<blender::ccl::CyclesRenderer*>(renderer.get()));
    if (!server.run()) {
        std::cerr << "Failed to start API server" << std::endl;
        return 1;
    }

    server.waitForShutdown();
    return 0;
}
