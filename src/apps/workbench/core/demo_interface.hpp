#pragma once

// Demo interface for workbench demos
// This provides a common interface for all demo types

// Forward declarations to minimize dependencies
namespace sep {
    namespace core {
        class Engine;
    }
    class SimpleRenderer;
    namespace quantum {
        class PatternMetricEngine;
    }
}

namespace sep::workbench {
    
    // Base demo interface
    class DemoInterface {
    public:
        virtual ~DemoInterface() = default;
        
        // Core lifecycle methods
        virtual void on_load(sep::core::Engine* engine, sep::SimpleRenderer* renderer) = 0;
        virtual void on_unload() = 0;
        virtual void on_update(float dt) = 0;
        virtual void on_render() = 0;
        virtual void on_ui_render() = 0; // For ImGui controls
        virtual void on_key_press(int key) = 0;
        
        // Optional pattern metric engine integration
        virtual void setPatternMetricEngine(sep::quantum::PatternMetricEngine* engine) {}
    };
    
    // Alias for backward compatibility
    using Demo = DemoInterface;
    
} // namespace sep::workbench
