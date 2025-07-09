#ifndef SEP_WORKBENCH_MEMORY_GARDEN_DEMO_HPP
#define SEP_WORKBENCH_MEMORY_GARDEN_DEMO_HPP

#include "demo.hpp"
#include <imgui.h>
#include <vector>
#include <glm/vec3.hpp>

namespace sep {
namespace workbench {

class MemoryGardenDemo : public Demo {
public:
    void on_load() override;
    void on_unload() override {}
    void on_update(float dt) override;
    void on_render() override;
    void on_key_press(int key) override {}

private:
    struct Node { glm::vec3 pos; };
    std::vector<Node> nodes_;
    float tier_radius_[3] = {5.f, 10.f, 15.f};
};

} // namespace workbench
} // namespace sep

#endif // SEP_WORKBENCH_MEMORY_GARDEN_DEMO_HPP
