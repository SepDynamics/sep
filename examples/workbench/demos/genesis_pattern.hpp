#pragma once

#include "demo.hpp"
#include <vector>

namespace sep {
namespace workbench {

class GenesisPatternDemo : public Demo {
public:
    void on_load() override;
    void on_update(float dt) override;
    void on_render() override;

private:
    void evolve(float dt);

    float evolution_rate_{0.2f};
    bool auto_evolve_{true};
    std::vector<float> dummy_state_;
};

} // namespace workbench
} // namespace sep
