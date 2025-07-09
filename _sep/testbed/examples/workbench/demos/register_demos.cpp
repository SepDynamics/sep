#include "demo_manager.hpp"
#include "genesis_pattern_demo.hpp"
#include "audio_visualizer_demo.hpp"
#include "memory_garden_demo.hpp"

namespace sep {
namespace workbench {

void register_demos() {
    auto& mgr = DemoManager::instance();
    mgr.register_demo("genesis", std::make_unique<GenesisPatternDemo>());
    mgr.register_demo("audio", std::make_unique<AudioVisualizerDemo>());
    mgr.register_demo("memory", std::make_unique<MemoryGardenDemo>());
}

} // namespace workbench
} // namespace sep
