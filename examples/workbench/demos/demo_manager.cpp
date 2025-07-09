#include "demo_manager.hpp"

namespace sep {
namespace workbench {

DemoManager& DemoManager::instance() {
    static DemoManager inst;
    return inst;
}

void DemoManager::register_demo(const std::string& key, DemoFactory factory) {
    factories_[key] = std::move(factory);
}

bool DemoManager::switch_demo(const std::string& key) {
    auto it = factories_.find(key);
    if (it == factories_.end()) return false;

    if (current_) current_->on_unload();
    current_ = it->second();
    if (current_) current_->on_load();
    return static_cast<bool>(current_);
}

void DemoManager::update(float dt) {
    if (current_) current_->on_update(dt);
}

void DemoManager::render() {
    if (current_) current_->on_render();
}

} // namespace workbench
} // namespace sep
