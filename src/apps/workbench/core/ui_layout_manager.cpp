#include "ui_layout_manager.h"

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

#include "imgui.h"

namespace sep::workbench {

UILayoutManager::UILayoutManager() {
    updateViewport();
}

void UILayoutManager::registerPanel(const UIPanel& panel) {
    panels_[panel.id] = panel;
    std::cout << "[UILayoutManager] Registered panel: " << panel.title << std::endl;
}

void UILayoutManager::unregisterPanel(const std::string& panel_id) {
    panels_.erase(panel_id);
}

void UILayoutManager::setPanelVisible(const std::string& panel_id, bool visible) {
    if (panels_.find(panel_id) != panels_.end()) {
        panels_[panel_id].visible = visible;
    }
}

void UILayoutManager::setPanelCallback(const std::string& panel_id, std::function<void()> callback) {
    if (panels_.find(panel_id) != panels_.end()) {
        panels_[panel_id].render_callback = callback;
    }
}

void UILayoutManager::createLayoutGroup(const std::string& group_name, PanelGroup group_type, 
                                       ImVec2 position, ImVec2 size) {
    LayoutGroup group;
    group.name = group_name;
    group.group_type = group_type;
    group.position = position;
    group.size = size;
    group.padding = global_padding_;
    group.spacing = global_spacing_;
    
    // Set group-specific styling
    switch (group_type) {
        case PanelGroup::MAIN_CHART:
            group.bg_color = ImVec4(0.08f, 0.08f, 0.12f, 0.95f);
            group.border_color = ImVec4(0.4f, 0.4f, 0.5f, 1.0f);
            break;
        case PanelGroup::TRADING_PANEL:
            group.bg_color = ImVec4(0.06f, 0.08f, 0.06f, 0.9f);
            group.border_color = ImVec4(0.2f, 0.6f, 0.2f, 1.0f);
            break;
        case PanelGroup::METRICS_PANEL:
            group.bg_color = ImVec4(0.08f, 0.06f, 0.08f, 0.9f);
            group.border_color = ImVec4(0.6f, 0.2f, 0.6f, 1.0f);
            break;
        case PanelGroup::ORDER_PANEL:
            group.bg_color = ImVec4(0.08f, 0.06f, 0.06f, 0.9f);
            group.border_color = ImVec4(0.6f, 0.4f, 0.2f, 1.0f);
            break;
        case PanelGroup::ANALYSIS_PANEL:
            group.bg_color = ImVec4(0.06f, 0.06f, 0.1f, 0.9f);
            group.border_color = ImVec4(0.2f, 0.4f, 0.8f, 1.0f);
            break;
        case PanelGroup::ALERTS_PANEL:
            group.bg_color = ImVec4(0.1f, 0.06f, 0.06f, 0.9f);
            group.border_color = ImVec4(0.8f, 0.2f, 0.2f, 1.0f);
            break;
    }
    
    layout_groups_[group_name] = group;
}

void UILayoutManager::addPanelToGroup(const std::string& group_name, const std::string& panel_id) {
    if (layout_groups_.find(group_name) != layout_groups_.end()) {
        layout_groups_[group_name].panel_ids.push_back(panel_id);
    }
}

void UILayoutManager::setGroupCollapsed(const std::string& group_name, bool collapsed) {
    if (layout_groups_.find(group_name) != layout_groups_.end()) {
        layout_groups_[group_name].collapsed = collapsed;
    }
}

void UILayoutManager::render() {
    updateViewport();
    
    if (customization_mode_) {
        renderCustomizationControls();
    }
    
    renderTabs();

    // Render content for the active tab
    if (active_tab_ == 0) { // Signals
        renderLayoutGroup("Main Chart");
    } else if (active_tab_ == 1) { // Engine
        renderLayoutGroup("SEP Metrics");
    } else if (active_tab_ == 2) { // Backend
        renderLayoutGroup("Data & Backtesting");
    }
}

void UILayoutManager::renderLayoutGroup(const std::string& group_name) {
    auto it = layout_groups_.find(group_name);
    if (it == layout_groups_.end()) return;
    
    LayoutGroup& group = it->second;
    
    // Set group window position and size
    ImGui::SetNextWindowPos(group.position, ImGuiCond_Always);
    ImGui::SetNextWindowSize(group.size, ImGuiCond_Always);
    
    // Group window flags
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
    if (!group.show_header) {
        flags |= ImGuiWindowFlags_NoTitleBar;
    }
    
    // Begin group window
    ImGui::PushStyleColor(ImGuiCol_WindowBg, group.bg_color);
    ImGui::PushStyleColor(ImGuiCol_Border, group.border_color);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(group.padding, group.padding));
    
    bool group_open = true;
    if (ImGui::Begin(group.name.c_str(), &group_open, flags)) {
        if (group.collapsible) {
            renderGroupHeader(group);
        }
        
        if (!group.collapsed) {
            // Calculate available space for panels
            ImVec2 content_region = ImGui::GetContentRegionAvail();
            float available_height = content_region.y;
            
            // Count visible panels in this group
            int visible_count = 0;
            for (const std::string& panel_id : group.panel_ids) {
                if (panels_.find(panel_id) != panels_.end() && panels_[panel_id].visible) {
                    visible_count++;
                }
            }
            
            if (visible_count > 0) {
                // Render panels in this group
                float panel_height = (available_height - (visible_count - 1) * group.spacing) / visible_count;
                
                bool first_panel = true;
                for (const std::string& panel_id : group.panel_ids) {
                    if (panels_.find(panel_id) != panels_.end() && panels_[panel_id].visible) {
                        if (!first_panel) {
                            ImGui::Spacing();
                        }
                        
                        // Create child window for panel
                        ImVec2 panel_size = ImVec2(content_region.x, panel_height);
                        
                        if (ImGui::BeginChild(panel_id.c_str(), panel_size, true)) {
                            renderPanel(panel_id);
                        }
                        ImGui::EndChild();
                        
                        first_panel = false;
                    }
                }
            }
        }
    }
    ImGui::End();
    
    ImGui::PopStyleVar(1); // WindowPadding
    ImGui::PopStyleColor(2); // WindowBg, Border
}

void UILayoutManager::renderPanel(const std::string& panel_id) {
    auto it = panels_.find(panel_id);
    if (it == panels_.end()) return;
    
    UIPanel& panel = it->second;
    
    if (!panel.visible) return;
    
    // Panel header
    ImGui::TextColored(ImVec4(0.8f, 0.9f, 1.0f, 1.0f), "%s", panel.title.c_str());
    ImGui::Separator();
    
    // Panel content
    if (panel.render_callback) {
        panel.render_callback();
    } else {
        ImGui::Text("Panel content for %s", panel.id.c_str());
        ImGui::Text("Priority: %d", static_cast<int>(panel.priority));
    }
}

void UILayoutManager::renderGroupHeader(const LayoutGroup& group) {
    ImGui::PushStyleColor(ImGuiCol_Text, group.border_color);
    if (ImGui::CollapsingHeader(("▼ " + group.name).c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
        // Group is expanded
    }
    ImGui::PopStyleColor();
}

void UILayoutManager::renderCustomizationControls() {
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 400), ImGuiCond_FirstUseEver);
    
    if (ImGui::Begin("Layout Customization")) {
        ImGui::Text("UI Layout Manager");
        ImGui::Separator();
        
        // Global settings
        if (ImGui::TreeNode("Global Settings")) {
            ImGui::SliderFloat("Padding", &global_padding_, 0.0f, 20.0f);
            ImGui::SliderFloat("Spacing", &global_spacing_, 0.0f, 10.0f);
            ImGui::TreePop();
        }
        
        // Layout presets
        if (ImGui::TreeNode("Layout Presets")) {
            if (ImGui::Button("Trading Layout")) { applyTradingLayout(); }
            if (ImGui::Button("Analysis Layout")) { applyAnalysisLayout(); }
            if (ImGui::Button("Minimal Layout")) { applyMinimalLayout(); }
            if (ImGui::Button("Development Layout")) { applyDevelopmentLayout(); }
            if (ImGui::Button("Reset to Default")) { resetToDefault(); }
            ImGui::TreePop();
        }
        
        // Panel visibility controls
        if (ImGui::TreeNode("Panel Visibility")) {
            for (auto& [panel_id, panel] : panels_) {
                ImGui::Checkbox(panel.title.c_str(), &panel.visible);
            }
            ImGui::TreePop();
        }
        
        // Group controls
        if (ImGui::TreeNode("Group Controls")) {
            for (auto& [group_name, group] : layout_groups_) {
                if (ImGui::TreeNode(group_name.c_str())) {
                    ImGui::Checkbox("Collapsed", &group.collapsed);
                    ImGui::ColorEdit4("Border Color", &group.border_color.x);
                    ImGui::ColorEdit4("Background Color", &group.bg_color.x);
                    ImGui::SliderFloat("Padding", &group.padding, 0.0f, 20.0f);
                    ImGui::SliderFloat("Spacing", &group.spacing, 0.0f, 10.0f);
                    ImGui::TreePop();
                }
            }
            ImGui::TreePop();
        }
    }
    ImGui::End();
}

void UILayoutManager::updateViewport() {
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    viewport_size_ = viewport->Size;
}

// Layout Presets Implementation
void UILayoutManager::applyTradingLayout() {
    layout_groups_.clear();
    
    // Main chart area (70% width, full height)
    createLayoutGroup("Main Chart", PanelGroup::MAIN_CHART, 
                     ImVec2(10, 30), ImVec2(viewport_size_.x * 0.7f - 20, viewport_size_.y - 60));
    
    // Right panel top (30% width, 40% height) - Trading controls
    createLayoutGroup("Trading", PanelGroup::TRADING_PANEL,
                     ImVec2(viewport_size_.x * 0.7f, 30), 
                     ImVec2(viewport_size_.x * 0.3f - 10, viewport_size_.y * 0.4f));
    
    // Right panel middle (30% width, 35% height) - SEP metrics
    createLayoutGroup("SEP Metrics", PanelGroup::METRICS_PANEL,
                     ImVec2(viewport_size_.x * 0.7f, 30 + viewport_size_.y * 0.4f + 10), 
                     ImVec2(viewport_size_.x * 0.3f - 10, viewport_size_.y * 0.35f));
    
    // Right panel bottom (30% width, 20% height) - Orders
    createLayoutGroup("Orders", PanelGroup::ORDER_PANEL,
                     ImVec2(viewport_size_.x * 0.7f, 30 + viewport_size_.y * 0.75f + 20), 
                     ImVec2(viewport_size_.x * 0.3f - 10, viewport_size_.y * 0.2f));
}

void UILayoutManager::applyAnalysisLayout() {
    layout_groups_.clear();
    
    // Main chart (50% width, full height)
    createLayoutGroup("Chart Analysis", PanelGroup::MAIN_CHART,
                     ImVec2(10, 30), ImVec2(viewport_size_.x * 0.5f - 15, viewport_size_.y - 60));
    
    // Right metrics (25% width, full height)
    createLayoutGroup("Advanced Metrics", PanelGroup::METRICS_PANEL,
                     ImVec2(viewport_size_.x * 0.5f, 30), 
                     ImVec2(viewport_size_.x * 0.25f - 5, viewport_size_.y - 60));
    
    // Far right analysis (25% width, full height)
    createLayoutGroup("Analysis Tools", PanelGroup::ANALYSIS_PANEL,
                     ImVec2(viewport_size_.x * 0.75f, 30), 
                     ImVec2(viewport_size_.x * 0.25f - 10, viewport_size_.y - 60));
}

void UILayoutManager::applyMinimalLayout() {
    layout_groups_.clear();
    
    // Full screen chart with small overlay panel
    createLayoutGroup("Minimal Chart", PanelGroup::MAIN_CHART,
                     ImVec2(10, 30), ImVec2(viewport_size_.x - 20, viewport_size_.y - 60));
    
    // Small overlay for essential controls
    createLayoutGroup("Essential Controls", PanelGroup::TRADING_PANEL,
                     ImVec2(viewport_size_.x - 320, 40), ImVec2(300, 200));
}

void UILayoutManager::applyDevelopmentLayout() {
    layout_groups_.clear();
    
    // Chart (60% width, 70% height)
    createLayoutGroup("Development Chart", PanelGroup::MAIN_CHART,
                     ImVec2(10, 30), 
                     ImVec2(viewport_size_.x * 0.6f - 15, viewport_size_.y * 0.7f - 35));
    
    // Metrics (40% width, 70% height)
    createLayoutGroup("Debug Metrics", PanelGroup::METRICS_PANEL,
                     ImVec2(viewport_size_.x * 0.6f, 30), 
                     ImVec2(viewport_size_.x * 0.4f - 10, viewport_size_.y * 0.7f - 35));
    
    // Bottom analysis panel (full width, 30% height)
    createLayoutGroup("Debug Analysis", PanelGroup::ANALYSIS_PANEL,
                     ImVec2(10, viewport_size_.y * 0.7f + 5), 
                     ImVec2(viewport_size_.x - 20, viewport_size_.y * 0.3f - 35));
}

void UILayoutManager::resetToDefault() {
    applyTradingLayout();
}

void UILayoutManager::setGroupStyle(const std::string& group_name, ImVec4 border_color, ImVec4 bg_color) {
    if (layout_groups_.find(group_name) != layout_groups_.end()) {
        layout_groups_[group_name].border_color = border_color;
        layout_groups_[group_name].bg_color = bg_color;
    }
}

void UILayoutManager::setGlobalSpacing(float padding, float spacing) {
    global_padding_ = padding;
    global_spacing_ = spacing;
    
    // Update all groups
    for (auto& [name, group] : layout_groups_) {
        group.padding = padding;
        group.spacing = spacing;
    }
}

void UILayoutManager::renderTabs() {
    if (ImGui::BeginTabBar("MainTabs")) {
        if (ImGui::BeginTabItem("SIGNALS")) {
            active_tab_ = 0;
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("ENGINE")) {
            active_tab_ = 1;
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("BACKEND")) {
            active_tab_ = 2;
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
}

} // namespace sep::workbench
