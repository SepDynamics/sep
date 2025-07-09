#pragma once

#include "demo.hpp"
#include <unordered_map>
#include <string>

namespace sep {
namespace workbench {

class DemoManager {
public:
    static DemoManager& instance();

    void register_demo(const std::string& key, DemoFactory factory);
    bool switch_demo(const std::string& key);

    void update(float dt);
    void render();

private:
    DemoManager() = default;

    std::unordered_map<std::string, DemoFactory> factories_;
    std::unique_ptr<Demo> current_;
};

} // namespace workbench
} // namespace sep
