#include "demos/demo_manager.hpp"
#include "demos/genesis_pattern.hpp"
#include "demos/audio_visualizer.hpp"
#include "demos/memory_garden.hpp"

int main() {
    using namespace sep::workbench;
    auto& mgr = DemoManager::instance();
    mgr.register_demo("genesis", []{ return std::make_unique<GenesisPatternDemo>(); });
    mgr.register_demo("audio", []{ return std::make_unique<AudioVisualizerDemo>(); });
    mgr.register_demo("memory", []{ return std::make_unique<MemoryGardenDemo>(); });

    mgr.switch_demo("genesis");
    // placeholder loop
    for(int i=0;i<1;++i) {
        mgr.update(0.016f);
        mgr.render();
    }
    return 0;
}
