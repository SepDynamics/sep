#include "api/server.h"
#include "core/manager.h"

int main(int argc, char** argv) {
    sep::config::ConfigManager::getInstance().initialize(argc, argv);
    const auto& api_cfg = sep::config::ConfigManager::getInstance().getAPIConfig();

    sep::api::SEPApiServer server(api_cfg);
    if (!server.run()) {
        return 1;
    }
    server.waitForShutdown();
    return 0;
}
