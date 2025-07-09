#ifndef SEP_WORKBENCH_GENESIS_PATTERN_DEMO_HPP
#define SEP_WORKBENCH_GENESIS_PATTERN_DEMO_HPP

#include "demo.hpp"
#include <imgui.h>

namespace sep {
namespace workbench {

class GenesisPatternDemo : public Demo {
public:
    void on_load() override;
    void on_unload() override {}
    void on_update(float dt) override;
    void on_render() override;
    void on_key_press(int key) override;

private:
    bool auto_evolve_ = true;
    float evolution_rate_ = 0.1f;
};

} // namespace workbench
} // namespace sep

#endif // SEP_WORKBENCH_GENESIS_PATTERN_DEMO_HPP
