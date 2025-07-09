#include "demo_manager.hpp"

namespace sep {
namespace workbench {

DemoManager& DemoManager::instance() {
    static DemoManager inst;
    return inst;
}

void DemoManager::registerDemo(const std::string& key,
                               std::function<std::unique_ptr<Demo>()> factory) {
    factories_[key] = std::move(factory);
}

bool DemoManager::switchToDemo(const std::string& key) {
    auto it = factories_.find(key);
    if (it == factories_.end()) {
        return false;
    }

    if (active_demo_) {
        active_demo_->on_unload();
    }

    active_demo_ = it->second();
    active_key_ = key;

    if (active_demo_) {
        active_demo_->on_load();
        return true;
    }
    return false;
}

void DemoManager::update(float dt) {
    if (active_demo_) {
        active_demo_->on_update(dt);
    }
}

void DemoManager::render() {
    if (active_demo_) {
        active_demo_->on_render();
    }
}

} // namespace workbench
} // namespace sep
