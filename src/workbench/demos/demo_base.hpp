#pragma once

// Forward declarations instead of including the full header
namespace sep {
    namespace core { class Engine; }
    namespace blender { class CyclesRenderer; }
}

namespace sep::workbench {
    class Demo {
    public:
        virtual ~Demo() = default;
        virtual void on_load(sep::core::Engine* engine, sep::blender::CyclesRenderer* renderer) = 0;
        virtual void on_unload() = 0;
        virtual void on_update(float dt) = 0;
        virtual void on_render() = 0;
        virtual void on_ui_render() = 0; // For ImGui controls
        virtual void on_key_press(int key) = 0;
    };
}