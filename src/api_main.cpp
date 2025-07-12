#include <iostream>
#include <memory>

#include "api/server.h"
#include "core/manager.h"
#include "sep_engine_wrapper.h"

int main(int argc, char** argv)
{
    auto& configMgr = sep::config::ConfigManager::getInstance();
    configMgr.initialize(argc, argv);

    // Create minimal renderer implementation
    std::unique_ptr<sep::CyclesRenderer> renderer = sep::createRenderer();
    if (renderer) {
        renderer.get()->initialize();
    }

    return 0;
}
