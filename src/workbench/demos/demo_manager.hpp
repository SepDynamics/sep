#pragma once

#ifndef SEP_WORKBENCH_DEMO
#define SEP_WORKBENCH_DEMO 1
#endif

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

#include "sep_engine_wrapper.h"
#include "workbench_demo_adapter.hpp"

namespace sep
{
    namespace workbench
    {


        class DemoManager
        {
        public:
            static DemoManager& getInstance()
            {
                static DemoManager instance;
                return instance;
            }

            void initialize(sep::Engine* engine, sep::CyclesRenderer* renderer);
            void registerDemo(const std::string& name,
                              std::function<std::unique_ptr<Demo>()> factory);
            bool switchToDemo(const std::string& name);
            void on_update(float dt);
            void on_render();
            void on_unload();
            void on_key(int key);

            const std::string& getCurrentDemo() const { return current_demo_name_; }

        private:
            DemoManager() = default;

            sep::Engine* engine_{nullptr};
            sep::CyclesRenderer* renderer_{nullptr};
            std::unordered_map<std::string, std::function<std::unique_ptr<Demo>()>> demo_factories_;
            std::unique_ptr<Demo> current_demo_;
            std::string current_demo_name_;
        };

    }  // namespace workbench
}  // namespace sep
