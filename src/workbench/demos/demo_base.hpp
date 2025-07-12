#include "workbench/demos/demo_base.hpp"
#pragma once
#include "sep_engine_wrapper.h" // For Engine and CyclesRenderer

namespace sep::workbench {
    class Demo {
    public:
        virtual ~Demo() = default;
        virtual void on_load(sep::Engine* engine, sep::CyclesRenderer* renderer) = 0;
        virtual void on_unload() = 0;
        virtual void on_update(float dt) = 0;
        virtual void on_render() = 0;
        virtual void on_ui_render() = 0; // For ImGui controls
        virtual void on_key_press(int key) = 0;
    };
}