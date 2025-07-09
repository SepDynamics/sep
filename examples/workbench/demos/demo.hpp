#pragma once

#include <memory>
#include <functional>

namespace sep {
namespace workbench {

class Demo {
public:
    virtual ~Demo() = default;
    virtual void on_load() {}
    virtual void on_unload() {}
    virtual void on_update(float dt) {}
    virtual void on_render() {}
};

using DemoFactory = std::function<std::unique_ptr<Demo>()>;

} // namespace workbench
} // namespace sep
