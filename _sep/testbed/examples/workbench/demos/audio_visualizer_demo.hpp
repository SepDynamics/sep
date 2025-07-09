#ifndef SEP_WORKBENCH_AUDIO_VISUALIZER_DEMO_HPP
#define SEP_WORKBENCH_AUDIO_VISUALIZER_DEMO_HPP

#include "demo.hpp"
#include <imgui.h>

namespace sep {
namespace workbench {

class AudioVisualizerDemo : public Demo {
public:
    void on_load() override;
    void on_unload() override {}
    void on_update(float dt) override;
    void on_render() override;
    void on_key_press(int key) override {}

private:
    float sensitivity_ = 1.0f;
};

} // namespace workbench
} // namespace sep

#endif // SEP_WORKBENCH_AUDIO_VISUALIZER_DEMO_HPP
