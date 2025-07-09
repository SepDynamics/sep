#pragma once

#include <memory>
#include <string>
#include <vector>

// Forward declarations instead of including mock_types.h
namespace sep
{
    namespace quantum
    {
        class Pattern;
    }

    // Forward declare Engine type
    namespace core
    {
        class Engine;
    }

    // SEP Engine Wrapper class
    class SepEngineWrapper
    {
    public:
        SepEngineWrapper();
        ~SepEngineWrapper();

        // Initialize the engine
        bool initialize();

        // Process patterns
        void processPatterns(const std::vector<quantum::Pattern>& patterns);

        // Get the underlying engine
        core::Engine* getEngine() { return engine_; }

    private:
        core::Engine* engine_{nullptr};
        bool initialized_{false};
    };

    // Global accessor
    SepEngineWrapper& getEngineWrapper();

}  // namespace sep