#include "demo_manager.hpp"
#include <stdexcept>

namespace sep {
namespace workbench {

void DemoManager::initialize(Engine* engine, CyclesRenderer* renderer) {
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
        current_demo_->cleanup();
    }

    current_demo_ = it->second();
    current_demo_name_ = name;

    if (current_demo_) {
        current_demo_->init();
        return true;
    }
    return false;
}

void DemoManager::update(float dt) {
    if (current_demo_) {
        current_demo_->update(dt);
    }
}

void DemoManager::render() {
    if (current_demo_) {
        current_demo_->render();
    }
}

void DemoManager::cleanup() {
    if (current_demo_) {
        current_demo_->cleanup();
        current_demo_.reset();
    }
}

void DemoManager::handleKeyboard(unsigned char key) {
    if (current_demo_) {
        current_demo_->handleKeyboard(key);
    }
}

void DemoManager::handleMouse(int x, int y, int button) {
    if (current_demo_) {
        current_demo_->handleMouse(x, y, button);
    }
}

} // namespace workbench
} // namespace sep