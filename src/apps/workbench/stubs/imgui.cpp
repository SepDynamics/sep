#include "imgui.h"
#include <cstdio>
#include <cstdarg>

namespace ImGui {
    bool Begin(const char* name) {
        if (!name) return false;
        printf("[ImGui] Begin: %s\n", name);
        return true;
    }

    void End() {
        // No-op
    }

    void Text(const char* fmt, ...) {
        if (!fmt) return;
        va_list args;
        va_start(args, fmt);
        printf("[ImGui] ");
        vprintf(fmt, args);
        printf("\n");
        va_end(args);
    }

    void Separator() {
        printf("[ImGui] ---\n");
    }

    bool Checkbox(const char* label, bool* v) {
        if (!label || !v) return false;
        printf("[ImGui] Checkbox: %s = %s\n", label, *v ? "true" : "false");
        return false;
    }

    bool SliderFloat(const char* label, float* v, float v_min, float v_max) {
        if (!label || !v) return false;
        printf("[ImGui] SliderFloat: %s = %.3f (%.3f - %.3f)\n", label, *v, v_min, v_max);
        return false;
    }

    bool Button(const char* label) {
        if (!label) return false;
        printf("[ImGui] Button: %s\n", label);
        return false;
    }
}
