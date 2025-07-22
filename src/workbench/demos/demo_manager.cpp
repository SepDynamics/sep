#include "demo_manager.hpp"

#include <iostream>

namespace sep {
namespace workbench {

    DemoManager& DemoManager::getInstance()
    {
        static DemoManager instance;
        return instance;
    }

    void DemoManager::initialize(sep::core::Engine* engine, sep::SimpleRenderer* renderer)
    {
        engine_ = engine;
        renderer_ = renderer;
    }

void DemoManager::registerDemo(const std::string& name, std::function<std::unique_ptr<Demo>()> factory) {
    demo_factories_[name] = std::move(factory);
}

bool DemoManager::switchToDemo(const std::string& name) {
    auto it = demo_factories_.find(name);
    if (it == demo_factories_.end()) return false;

    if (current_demo_) {
        current_demo_->on_unload();
    }

    current_demo_ = it->second();
    current_demo_->on_load(engine_, renderer_);
    current_demo_name_ = name;
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

void DemoManager::on_ui_render() {
    if (current_demo_) {
        current_demo_->on_ui_render();
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

std::string DemoManager::getCurrentDemo() const { return current_demo_name_; }

std::vector<std::string> DemoManager::getRegisteredDemos() const
{
    std::vector<std::string> names;
    names.reserve(demo_factories_.size());
    for (const auto& pair : demo_factories_)
        names.push_back(pair.first);
    return names;
}

}  // namespace workbench
}  // namespace sep