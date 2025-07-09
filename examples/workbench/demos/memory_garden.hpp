#pragma once

#include "demo.hpp"
#include <vector>

namespace sep {
namespace workbench {

class MemoryGardenDemo : public Demo {
public:
    void on_load() override;
    void on_update(float dt) override;
    void on_render() override;

private:
    struct Node { float coherence; };
    std::vector<Node> nodes_;
    float stm_radius_{10.f};
};

} // namespace workbench
} // namespace sep
