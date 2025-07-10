#include "demo_manager.hpp"

namespace sep {
namespace workbench {

void DemoManager::initialize(sep::Engine* engine, sep::CyclesRenderer* renderer) {
    engine_ = engine;
    renderer_ = renderer;
}

void DemoManager::registerDemo(const std::string& name, std::function<std::unique_ptr<Demo>()> factory) {
    demo_factories_[name] = std::move(factory);
}

bool DemoManager::switchToDemo(const std::string& name) {
    auto it = demo_factories_.find(name);
    if (it == demo_factories_.end()) {
        return false;
    }

    if (current_demo_) {
        current_demo_->on_unload();
    }

    current_demo_ = it->second();
    current_demo_name_ = name;
    current_demo_->initialize(engine_, renderer_);
    current_demo_->on_load();

    return true;
}

void DemoManager::on_update(float dt) {
    if (current_demo_) {
        current_demo_->on_update(dt);
    }
}

void DemoManager::on_render() {
    if (current_demo_) {
        current_demo_->on_render();
    }
}

void DemoManager::on_unload() {
    if (current_demo_) {
        current_demo_->on_unload();
        current_demo_.reset();
    }
}

void DemoManager::on_key(int key) {
    if (current_demo_) {
        current_demo_->on_key_press(key);
    }
}

} // namespace workbench
} // namespace sep
