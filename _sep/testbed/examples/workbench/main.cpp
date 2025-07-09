#include "demos/demo_manager.hpp"
#include "demos/register_demos.cpp"
#include <iostream>

int main() {
    sep::workbench::register_demos();
    auto& mgr = sep::workbench::DemoManager::instance();
    mgr.switch_to("genesis");
    mgr.update(0.0f);
    mgr.render();
    std::cout << "Testbed main executed" << std::endl;
    return 0;
}
