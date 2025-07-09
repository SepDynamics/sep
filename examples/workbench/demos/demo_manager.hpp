#pragma once

#ifndef SEP_WORKBENCH_DEMO
#define SEP_WORKBENCH_DEMO 1
#endif

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

#include "sep_engine_wrapper.h"

namespace sep
{
    namespace workbench
    {

        class Demo
        {
        public:
            virtual ~Demo() = default;
            virtual void on_load() = 0;
            virtual void on_unload() = 0;
            virtual void on_update(float dt) = 0;
            virtual void on_render() = 0;
            virtual void on_key_press(int key) = 0;
            virtual void on_mouse(int x, int y, int button) = 0;

            void initialize(sep::Engine* engine,
                            sep::CyclesRenderer* renderer);  // Declaration only

        protected:
            sep::Engine* engine_{nullptr};
            sep::CyclesRenderer* renderer_{nullptr};
        };

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
