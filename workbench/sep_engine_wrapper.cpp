#include "sep_engine_wrapper.h"
#include "quantum_pattern.hpp"

#include <iostream>

namespace sep
{

    // Dummy Engine implementation for the wrapper
    namespace core
    {
        class Engine
        {
        public:
            Engine() = default;
            ~Engine() = default;

            void initialize() { std::cout << "Engine initialized" << std::endl; }

            void processPatterns(const std::vector<quantum::Pattern>& patterns)
            {
                std::cout << "Processing " << patterns.size() << " patterns" << std::endl;
            }
        };
    }  // namespace core

    SepEngineWrapper::SepEngineWrapper() : engine_(nullptr), initialized_(false)
    {
        std::cout << "SepEngineWrapper created" << std::endl;
    }

    SepEngineWrapper::~SepEngineWrapper()
    {
        if (engine_)
        {
            delete engine_;
            engine_ = nullptr;
        }
        std::cout << "SepEngineWrapper destroyed" << std::endl;
    }

    bool SepEngineWrapper::initialize()
    {
        if (!initialized_)
        {
            engine_ = new core::Engine();
            engine_->initialize();
            initialized_ = true;
            std::cout << "SepEngineWrapper initialized" << std::endl;
        }
        return true;
    }

    void SepEngineWrapper::processPatterns(const std::vector<quantum::Pattern>& patterns)
    {
        if (engine_ && initialized_)
        {
            engine_->processPatterns(patterns);
        }
        else
        {
            std::cerr << "SepEngineWrapper not initialized!" << std::endl;
        }
    }

    // Global instance
    static SepEngineWrapper g_engine_wrapper;

    SepEngineWrapper& getEngineWrapper() { return g_engine_wrapper; }

}  // namespace sep