#include "demo_manager.hpp"

namespace sep {
namespace workbench {

DemoManager& DemoManager::instance() {
    static DemoManager inst;
    return inst;
}

void DemoManager::register_demo(const std::string& key, std::unique_ptr<Demo> demo) {
    demos_[key] = std::move(demo);
}

bool DemoManager::switch_to(const std::string& key) {
    auto it = demos_.find(key);
    if (it == demos_.end()) return false;
    if (active_) active_->on_unload();
    active_ = it->second.get();
    active_key_ = key;
    active_->on_load();
    return true;
}

void DemoManager::update(float dt) {
    if (active_) active_->on_update(dt);
}

void DemoManager::render() {
    if (active_) active_->on_render();
}

void DemoManager::key_press(int key) {
    if (active_) active_->on_key_press(key);
}

} // namespace workbench
} // namespace sep
