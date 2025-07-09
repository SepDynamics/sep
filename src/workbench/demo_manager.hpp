#pragma once

#include <memory>
#include <string>
#include <vector>

namespace sep
{

    // Forward declare required types
    namespace core
    {
        class Engine;
    }

    namespace blender
    {
        class CyclesRenderer;
    }

    namespace workbench
    {

        class Demo
        {
        public:
            virtual ~Demo() = default;
            virtual void init() = 0;
            virtual void update(float dt) = 0;
            virtual void render() = 0;
            virtual void cleanup() = 0;
            virtual void handleKeyboard(unsigned char key) = 0;
            virtual void handleMouse(int x, int y, int button) = 0;
        };

        class DemoManager
        {
        public:
            static DemoManager& getInstance();
            void initialize(sep::core::Engine* engine, sep::blender::CyclesRenderer* renderer);
            void registerDemo(const std::string& name, std::unique_ptr<Demo> demo);
            void setActiveDemo(const std::string& name);
            void update(float dt);
            void render();
            void cleanup();
            void handleKeyboard(unsigned char key);
            void handleMouse(int x, int y, int button);

        private:
            DemoManager() = default;
            sep::core::Engine* engine_{nullptr};
            sep::blender::CyclesRenderer* renderer_{nullptr};
            std::vector<std::pair<std::string, std::unique_ptr<Demo>>> demos_;
            std::size_t active_demo_index_{0};
        };

    }  // namespace workbench
}  // namespace sep