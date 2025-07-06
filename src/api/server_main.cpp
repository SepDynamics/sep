#include "api/server.h"
#include "core/manager.h"
#include "blender/cycles_renderer.h"
#include <memory>

int main(int argc, char** argv) {
    auto& cfg_manager = sep::config::ConfigManager::getInstance();
    cfg_manager.initialize(argc, argv);
    const auto& api_cfg = cfg_manager.getAPIConfig();

    auto renderer = std::make_unique<sep::blender::ccl::CyclesRenderer>();
    renderer->initialize();

    sep::api::SEPApiServer server(api_cfg, renderer.get());
    if (!server.run()) {
        return 1;
    }
    server.waitForShutdown();
    return 0;
}
