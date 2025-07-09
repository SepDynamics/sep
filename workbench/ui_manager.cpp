#include "ui_manager.h"

#include <iostream>

UIManager::UIManager() : window_handle(nullptr)
{
    std::cout << "UIManager initialized" << std::endl;
}

UIManager::~UIManager() { std::cout << "UIManager destroyed" << std::endl; }

bool UIManager::init(sep::workbench::Window& window)
{
    window_handle = window.getGLFWWindow();
    if (!window_handle)
    {
        std::cerr << "Failed to get window handle in UIManager" << std::endl;
        return false;
    }

    std::cout << "UIManager connected to window" << std::endl;
    return true;
}

void UIManager::beginFrame()
{
    // For now, just a stub - we'll add ImGui integration later
}

void UIManager::endFrame()
{
    // For now, just a stub - we'll add ImGui integration later
}

void UIManager::render()
{
    // For now, just a stub - we'll add ImGui integration later
}

void UIManager::showPatternControls(float& evolution_rate, bool& auto_evolve)
{
    // For now, just a stub - we'll implement proper UI controls later
    // Just print the current values to console
    std::cout << "Evolution rate: " << evolution_rate
              << " | Auto-evolve: " << (auto_evolve ? "ON" : "OFF") << std::endl;
}

void UIManager::showPatternMetrics(int pattern_count, float avg_coherence)
{
    // For now, just a stub - we'll implement proper UI metrics display later
    // Just print the values to console
    std::cout << "Patterns: " << pattern_count << " | Avg coherence: " << avg_coherence
              << std::endl;
}