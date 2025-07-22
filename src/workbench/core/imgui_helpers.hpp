#pragma once

#include <imgui.h>
#include <imgui_internal.h>
#include <cmath>

namespace ImGui {

// Spinner widget for loading indicators
inline void Spinner(const char* label, float radius, float thickness, const ImU32& color) {
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;
    
    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    
    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size((radius )*2, (radius + style.FramePadding.y)*2);
    
    const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
    ItemSize(bb, style.FramePadding.y);
    if (!ItemAdd(bb, id))
        return;
    
    // Render
    window->DrawList->PathClear();
    
    int num_segments = 30;
    int start = abs(ImSin(g.Time*1.8f)*(num_segments-5));
    
    const float a_min = IM_PI*2.0f * ((float)start) / (float)num_segments;
    const float a_max = IM_PI*2.0f * ((float)num_segments-3) / (float)num_segments;

    const ImVec2 centre = ImVec2(pos.x+radius, pos.y+radius+style.FramePadding.y);
    
    for (int i = 0; i < num_segments; i++) {
        const float a = a_min + ((float)i / (float)num_segments) * (a_max - a_min);
        window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(a+g.Time*8) * radius,
                                             centre.y + ImSin(a+g.Time*8) * radius));
    }

    window->DrawList->PathStroke(color, false, thickness);
}

// Progress bar with gradient
inline void GradientProgressBar(float fraction, const ImVec2& size_arg = ImVec2(-1, 0), 
                                const char* overlay = NULL) {
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size = CalcItemSize(size_arg, CalcItemWidth(), g.FontSize + style.FramePadding.y*2.0f);
    ImRect bb(pos, pos + size);
    ItemSize(size, style.FramePadding.y);
    if (!ItemAdd(bb, 0))
        return;

    // Render
    fraction = ImSaturate(fraction);
    RenderFrame(bb.Min, bb.Max, GetColorU32(ImGuiCol_FrameBg), true, style.FrameRounding);
    bb.Expand(ImVec2(-style.FrameBorderSize, -style.FrameBorderSize));
    const ImVec2 fill_br = ImVec2(ImLerp(bb.Min.x, bb.Max.x, fraction), bb.Max.y);

    // Gradient fill
    ImU32 col_start = GetColorU32(ImVec4(0.2f, 0.8f, 1.0f, 1.0f));
    ImU32 col_end = GetColorU32(ImVec4(0.8f, 0.2f, 1.0f, 1.0f));
    
    window->DrawList->AddRectFilledMultiColor(bb.Min, fill_br, 
        col_start, col_end, col_end, col_start);

    // Overlay text
    if (overlay != NULL) {
        ImVec2 overlay_size = CalcTextSize(overlay, NULL);
        if (overlay_size.x > 0.0f) {
            float text_x = bb.Min.x + (bb.Max.x - bb.Min.x - overlay_size.x) * 0.5f;
            float text_y = bb.Min.y + (bb.Max.y - bb.Min.y - overlay_size.y) * 0.5f;
            RenderTextClipped(ImVec2(text_x, text_y), bb.Max, overlay, NULL, &overlay_size);
        }
    }
}

// Animated text with wave effect
inline void WaveText(const char* text, float wave_speed = 2.0f, float wave_amplitude = 5.0f) {
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    ImGuiContext& g = *GImGui;
    ImVec2 pos = window->DC.CursorPos;
    
    const char* text_end = text + strlen(text);
    float offset_x = 0.0f;
    
    for (const char* p = text; p < text_end; ) {
        unsigned int c = (unsigned int)*p;
        if (c < 0x80) {
            float wave_offset = ImSin((g.Time * wave_speed) + offset_x * 0.1f) * wave_amplitude;
            window->DrawList->AddText(ImVec2(pos.x + offset_x, pos.y + wave_offset), 
                                    GetColorU32(ImGuiCol_Text), p, p + 1);
            
            ImVec2 char_size = CalcTextSize(p, p + 1);
            offset_x += char_size.x;
            p++;
        }
    }
    
    ItemSize(ImVec2(offset_x, GetTextLineHeight() + wave_amplitude * 2));
}

// Notification toast
inline void Toast(const char* title, const char* message, float display_time = 3.0f) {
    static float toast_alpha = 0.0f;
    static float toast_timer = 0.0f;
    static std::string toast_title;
    static std::string toast_message;
    
    if (title && message && display_time > 0.0f) {
        toast_title = title;
        toast_message = message;
        toast_timer = display_time;
        toast_alpha = 1.0f;
    }
    
    if (toast_timer > 0.0f) {
        ImGuiIO& io = GetIO();
        toast_timer -= io.DeltaTime;
        
        if (toast_timer < 0.5f) {
            toast_alpha = toast_timer * 2.0f;
        }
        
        SetNextWindowPos(ImVec2(io.DisplaySize.x - 320, io.DisplaySize.y - 120), 
                        ImGuiCond_Always, ImVec2(1.0f, 1.0f));
        SetNextWindowSize(ImVec2(300, 100));
        
        PushStyleVar(ImGuiStyleVar_Alpha, toast_alpha);
        Begin("##Toast", nullptr, ImGuiWindowFlags_NoDecoration | 
                                 ImGuiWindowFlags_NoInputs | 
                                 ImGuiWindowFlags_NoFocusOnAppearing);
        
        TextColored(ImVec4(1.0f, 0.8f, 0.2f, 1.0f), "%s", toast_title.c_str());
        Separator();
        TextWrapped("%s", toast_message.c_str());
        
        End();
        PopStyleVar();
    }
}

} // namespace ImGui