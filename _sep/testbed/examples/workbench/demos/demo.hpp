#ifndef SEP_WORKBENCH_DEMO_HPP
#define SEP_WORKBENCH_DEMO_HPP

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
} // namespace workbench
} // namespace sep

#endif // SEP_WORKBENCH_DEMO_HPP
