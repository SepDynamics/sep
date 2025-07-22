#pragma once

// Minimal ImGui stub for workbench standalone builds
namespace ImGui {
    bool Begin(const char* name);
    void End();
    void Text(const char* fmt, ...);
    void Separator();
    bool Checkbox(const char* label, bool* v);
    bool SliderFloat(const char* label, float* v, float v_min, float v_max);
    bool Button(const char* label);
}

// Basic types that ImGui demos expect
typedef int ImGuiWindowFlags;
#define ImGuiWindowFlags_None 0
