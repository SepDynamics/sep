#pragma once
#include "window.h"

class UIManager
{
public:
    UIManager();
    ~UIManager();

    bool init(sep::workbench::Window& window);
    void beginFrame();
    void endFrame();
    void render();

    // UI Components
    void showPatternControls(float& evolution_rate, bool& auto_evolve);
    void showPatternMetrics(int pattern_count, float avg_coherence);

private:
    GLFWwindow* window_handle;
};