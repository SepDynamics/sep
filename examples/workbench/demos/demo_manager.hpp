#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

namespace sep {
namespace workbench {

class Demo {
public:
    virtual ~Demo() = default;
    virtual void on_load() = 0;
    virtual void on_unload() = 0;
    virtual void on_update(float dt) = 0;
    virtual void on_render() = 0;
    virtual void on_key_press(int key) = 0;
};

class DemoManager {
public:
    static DemoManager& instance();

    void registerDemo(const std::string& key,
                      std::function<std::unique_ptr<Demo>()> factory);

    bool switchToDemo(const std::string& key);
    void update(float dt);
    void render();

private:
    DemoManager() = default;

    std::unordered_map<std::string, std::function<std::unique_ptr<Demo>()>> factories_;
    std::unique_ptr<Demo> active_demo_;
    std::string active_key_;
};

} // namespace workbench
} // namespace sep
