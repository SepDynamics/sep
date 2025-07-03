#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <functional>

#include "core/engine.h"
#include "blender/cycles_renderer.h"

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
    Engine* engine_{nullptr};
    CyclesRenderer* renderer_{nullptr};
};

class DemoManager {
public:
    static DemoManager& getInstance() {
        static DemoManager instance;
        return instance;
    }

    void initialize(Engine* engine, CyclesRenderer* renderer);
    void registerDemo(const std::string& name, std::function<std::unique_ptr<Demo>()> factory);
    bool switchToDemo(const std::string& name);
    void update(float dt);
    void render();
    void cleanup();
    void handleKeyboard(unsigned char key);
    void handleMouse(int x, int y, int button);

    const std::string& getCurrentDemo() const { return current_demo_; }

private:
    DemoManager() = default;

    Engine* engine_{nullptr};
    CyclesRenderer* renderer_{nullptr};
    std::unordered_map<std::string, std::function<std::unique_ptr<Demo>()>> demo_factories_;
    std::unique_ptr<Demo> current_demo_;
    std::string current_demo_name_;
};

} // namespace workbench
} // namespace sep