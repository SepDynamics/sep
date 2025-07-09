#include "demo_manager.hpp"

namespace sep
{
    namespace workbench
    {

        DemoManager& DemoManager::getInstance()
        {
            static DemoManager instance;
            return instance;
        }

        void DemoManager::initialize(sep::core::Engine* engine,
                                     sep::blender::CyclesRenderer* renderer)
        {
            engine_ = engine;
            renderer_ = renderer;
        }

        void DemoManager::registerDemo(const std::string& name, std::unique_ptr<Demo> demo)
        {
            demos_.emplace_back(name, std::move(demo));
        }

        void DemoManager::setActiveDemo(const std::string& name)
        {
            for (std::size_t i = 0; i < demos_.size(); ++i)
            {
                if (demos_[i].first == name)
                {
                    active_demo_index_ = i;
                    demos_[active_demo_index_].second->init();
                    return;
                }
            }
        }

        void DemoManager::update(float dt)
        {
            if (!demos_.empty())
            {
                demos_[active_demo_index_].second->update(dt);
            }
        }

        void DemoManager::render()
        {
            if (!demos_.empty())
            {
                demos_[active_demo_index_].second->render();
            }
        }

        void DemoManager::cleanup()
        {
            if (!demos_.empty())
            {
                demos_[active_demo_index_].second->cleanup();
            }
            demos_.clear();
        }

        void DemoManager::handleKeyboard(unsigned char key)
        {
            if (!demos_.empty())
            {
                demos_[active_demo_index_].second->handleKeyboard(key);
            }
        }

        void DemoManager::handleMouse(int x, int y, int button)
        {
            if (!demos_.empty())
            {
                demos_[active_demo_index_].second->handleMouse(x, y, button);
            }
        }

    }  // namespace workbench
}  // namespace sep