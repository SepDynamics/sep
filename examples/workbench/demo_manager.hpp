#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <functional>

#include "sep_engine_wrapper.h"
#include "core/engine.h"
#include <glm/glm.hpp>


namespace sep {
namespace workbench {

class Demo {
public:
    virtual ~Demo() = default;
    virtual void init() = 0;
    virtual void update(float dt) = 0;
    virtual void render() = 0;
    virtual void cleanup() = 0;
    virtual void handleKeyboard(unsigned char key) = 0;
    virtual void handleMouse(int x, int y, int button) = 0;

protected:
    sep::Engine* engine_{nullptr};
    sep::CyclesRenderer* renderer_{nullptr};
    
    // Add access to the config manager
    sep::core::config::ConfigManager& getConfigManager() {
        return sep::core::config::ConfigManager::getInstance();
    }
};

class DemoManager {
public:
    static DemoManager& getInstance() {
        static DemoManager instance;
        return instance;
    }

    void initialize(sep::Engine* engine, sep::CyclesRenderer* renderer);
    void registerDemo(const std::string& name, std::function<std::unique_ptr<Demo>()> factory);
    bool switchToDemo(const std::string& name);
    void update(float dt);
    void render();
    void cleanup();
    void handleKeyboard(unsigned char key);
    void handleMouse(int x, int y, int button);

    const std::string& getCurrentDemo() const { return current_demo_name_; }

private:
    DemoManager() = default;

    sep::Engine* engine_{nullptr};
    sep::CyclesRenderer* renderer_{nullptr};
    std::unordered_map<std::string, std::function<std::unique_ptr<Demo>()>> demo_factories_;
    std::unique_ptr<Demo> current_demo_;
    std::string current_demo_name_;
};

} // namespace workbench
} // namespace sep