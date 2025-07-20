#pragma once

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "demo_base.hpp"

namespace sep
{
    namespace workbench
    {

        class DemoManager
        {
        public:
            static DemoManager& getInstance();
            void initialize(sep::Engine* engine, sep::CyclesRenderer* renderer);
            void registerDemo(const std::string& name,
                              std::function<std::unique_ptr<Demo>()> factory);
            bool switchToDemo(const std::string& name);
            void on_update(float dt);
            void on_render();
            void on_ui_render();
            void on_unload();
            void on_key(int key);
            std::string getCurrentDemo() const;
            std::vector<std::string> getRegisteredDemos() const;

        private:
            DemoManager() = default;  // Singleton
            ~DemoManager() = default;

            sep::Engine* engine_{nullptr};
            sep::CyclesRenderer* renderer_{nullptr};
            std::map<std::string, std::function<std::unique_ptr<Demo>()>> demo_factories_;
            std::unique_ptr<Demo> current_demo_;
            std::string current_demo_name_;
        };

    }  // namespace workbench
}  // namespace sep