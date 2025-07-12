#include <iostream>
#include <memory>

#include "api/server.h"
#include "core/manager.h"
#include "sep_engine_wrapper.h"

int main(int argc, char** argv)
{
    auto& configMgr = sep::config::ConfigManager::getInstance();
    configMgr.initialize(argc, argv);
    const auto& apiConfig = configMgr.getAPIConfig();

    // Create minimal renderer implementation
    std::unique_ptr<sep::blender::ccl::CyclesRenderer> renderer = sep::createRenderer();
    if (renderer) {
        renderer->initialize();
    }

    sep::api::SEPApiServer server(apiConfig, renderer.get());
    if (!server.run()) {
        std::cerr << "Failed to start API server" << std::endl;
        return 1;
    }

    server.waitForShutdown();
    return 0;
}
