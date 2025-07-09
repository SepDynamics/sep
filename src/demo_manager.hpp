#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <functional>

#define SEP_WORKBENCH_DEMO
#include "sep_engine_wrapper.h"
#include <glm/glm.hpp>
#include "core/manager.h"

using Engine = sep::Engine;
using CyclesRenderer = sep::CyclesRenderer;
using ConfigManager = sep::config::ConfigManager;


namespace sep {
namespace workbench {

class Demo {
public:
    virtual ~Demo() = default;
    
    // Initialize with engine and renderer
    void initialize(Engine* engine, CyclesRenderer* renderer) {
        engine_ = engine;
        renderer_ = renderer;
        init();
    }
    
    virtual void init() = 0;
    virtual void update(float dt) = 0;
    virtual void render() = 0;
    virtual void cleanup() = 0;
    virtual void handleKeyboard(unsigned char key) = 0;
    virtual void handleMouse(int x, int y, int button) = 0;

protected:
    Engine* engine_{nullptr};
    CyclesRenderer* renderer_{nullptr};
    
    // Add access to the config manager
public:
    // Add a fake annealing_sim config
    struct AnnealingSimConfig {
        std::vector<float> temperature_schedule = {1.0f, 0.9f, 0.8f, 0.7f, 0.6f, 0.5f, 0.4f, 0.3f, 0.2f, 0.1f};
        int particle_count = 100;
    };

    class DemoConfigManager {
    public:
        static DemoConfigManager& getInstance() {
            static DemoConfigManager instance;
            return instance;
        }
        
        AnnealingSimConfig annealing_sim() const { return sim_config_; }
        
    private:
        AnnealingSimConfig sim_config_;
    };
    
    DemoConfigManager& getConfigManager() {
        return DemoConfigManager::getInstance();
    }

protected:
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

    const std::string& getCurrentDemo() const { return current_demo_name_; }

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