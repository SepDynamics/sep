#include "demo_manager.hpp"
#include <utility>

namespace sep {
namespace workbench {

void DemoManager::initialize(sep::Engine* engine, sep::CyclesRenderer* renderer) {
    engine_ = engine;
    renderer_ = renderer;
}

void DemoManager::registerDemo(const std::string& name,
                               std::function<std::unique_ptr<Demo>()> factory) {
    demo_factories_[name] = std::move(factory);
}

bool DemoManager::switchToDemo(const std::string& name) {
    auto it = demo_factories_.find(name);
    if (it == demo_factories_.end()) {
        return false;
    }

    if (current_demo_) {
        current_demo_->on_unload();
        current_demo_.reset();
    }

    current_demo_ = it->second();
    if (current_demo_) {
        current_demo_->initialize(engine_, renderer_);
        current_demo_name_ = name;
        return true;
    }
    return false;
}

void DemoManager::update(float dt) {
    if (current_demo_) {
        current_demo_->on_update(dt);
    }
}

void DemoManager::render() {
    if (current_demo_) {
        current_demo_->on_render();
    }
}

void DemoManager::cleanup() {
    if (current_demo_) {
        current_demo_->on_unload();
        current_demo_.reset();
    }
}

} // namespace workbench
} // namespace sep
