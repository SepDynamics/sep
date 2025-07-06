#include "api/server.h"
#include "core/manager.h"
#include <memory>

int main(int argc, char** argv) {
    auto& cfg_manager = sep::config::ConfigManager::getInstance();
    cfg_manager.initialize(argc, argv);
    const auto& api_cfg = cfg_manager.getAPIConfig();

    sep::api::SEPApiServer server(api_cfg);
    if (!server.run()) {
        return 1;
    }
    server.waitForShutdown();
    return 0;
}
