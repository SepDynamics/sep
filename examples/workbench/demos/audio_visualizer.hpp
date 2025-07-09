#pragma once

#include "demo.hpp"
#include <vector>

namespace sep {
namespace workbench {

class AudioVisualizerDemo : public Demo {
public:
    void on_load() override;
    void on_update(float dt) override;
    void on_render() override;

private:
    std::vector<float> spectrum_;
    float sensitivity_{1.0f};
};

} // namespace workbench
} // namespace sep
