#ifndef SEP_WORKBENCH_DEMO_MANAGER_HPP
#define SEP_WORKBENCH_DEMO_MANAGER_HPP

#include <memory>
#include <string>
#include <unordered_map>
#include "demo.hpp"

namespace sep {
namespace workbench {
class DemoManager {
public:
    static DemoManager& instance();

    void register_demo(const std::string& key, std::unique_ptr<Demo> demo);
    bool switch_to(const std::string& key);
    void update(float dt);
    void render();
    void key_press(int key);

private:
    std::unordered_map<std::string, std::unique_ptr<Demo>> demos_;
    Demo* active_{nullptr};
    std::string active_key_;
};
} // namespace workbench
} // namespace sep

#endif // SEP_WORKBENCH_DEMO_MANAGER_HPP
