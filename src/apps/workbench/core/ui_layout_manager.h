#pragma once

#include <functional>
#include <map>
#include <string>
#include <vector>
#include <unordered_map>
#include <typeindex>

#include "imgui.h"
#include "connectors/oanda_connector.h"
#include "service_connector.hpp"

namespace sep::workbench {

/**
 * Organized UI Layout Manager with Customizable Panels and Subgroups
 * Provides template-based layout system with boundaries and grouping
 */

enum class PanelGroup {
    MAIN_CHART,      // Primary chart area
    TRADING_PANEL,   // Trading controls and account info
    METRICS_PANEL,   // SEP metrics and analysis
    ORDER_PANEL,     // Order management and positions
    ANALYSIS_PANEL,  // Advanced analysis and tools
    ALERTS_PANEL     // Alerts and notifications
};

enum class PanelPriority {
    CRITICAL = 1,    // Always visible, core functionality
    HIGH = 2,        // Important panels, visible by default
    MEDIUM = 3,      // Optional panels, can be collapsed
    LOW = 4          // Debug/advanced panels, hidden by default
};

struct UIPanel {
    std::string id;
    std::string title;
    PanelGroup group;
    PanelPriority priority;
    bool visible = true;
    bool collapsible = true;
    bool resizable = true;
    ImVec2 min_size = ImVec2(200, 150);
    ImVec2 max_size = ImVec2(800, 600);
    ImVec2 default_size = ImVec2(300, 200);
    std::function<void()> render_callback;
    
    // Default constructor for STL containers
    UIPanel() : group(PanelGroup::MAIN_CHART), priority(PanelPriority::LOW) {}
    
    UIPanel(const std::string& panel_id, const std::string& panel_title, 
            PanelGroup panel_group, PanelPriority panel_priority)
        : id(panel_id), title(panel_title), group(panel_group), priority(panel_priority) {}
};

struct LayoutGroup {
    std::string name;
    PanelGroup group_type;
    ImVec2 position;
    ImVec2 size;
    ImVec4 border_color = ImVec4(0.3f, 0.3f, 0.4f, 1.0f);
    ImVec4 bg_color = ImVec4(0.06f, 0.06f, 0.08f, 0.9f);
    float padding = 8.0f;
    float spacing = 4.0f;
    bool show_header = true;
    bool collapsible = true;
    bool collapsed = false;
    std::vector<std::string> panel_ids;
};

// Simple event structures
struct PanelVisibilityEvent {
    std::string panel_id;
    bool visible{false};
};

struct GroupCollapsedEvent {
    std::string group_name;
    bool collapsed{false};
};



struct ConnectionStateEvent {
    sep::workbench::ConnectionState state;
};

struct OrderUpdateEvent {
    sep::connectors::OrderInfo info;
};

class EventBus {
public:
    template<typename EventType>
    void subscribe(std::function<void(const EventType&)> handler) {
        auto key = std::type_index(typeid(EventType));
        handlers_[key].push_back([handler](const void* e) {
            handler(*static_cast<const EventType*>(e));
        });
    }

    template<typename EventType>
    void publish(const EventType& event) {
        auto key = std::type_index(typeid(EventType));
        auto it = handlers_.find(key);
        if (it != handlers_.end()) {
            for (auto& cb : it->second) {
                cb(&event);
            }
        }
    }

private:
    std::unordered_map<std::type_index,
                       std::vector<std::function<void(const void*)>>> handlers_;
};

// Global event bus accessor
EventBus& globalEventBus();

class UILayoutManager {
public:
    UILayoutManager();
    ~UILayoutManager() = default;
    
    // Panel Management
    void registerPanel(const UIPanel& panel);
    void unregisterPanel(const std::string& panel_id);
    void setPanelVisible(const std::string& panel_id, bool visible);
    void setPanelCallback(const std::string& panel_id, std::function<void()> callback);
    
    // Layout Management
    void createLayoutGroup(const std::string& group_name, PanelGroup group_type, 
                          ImVec2 position, ImVec2 size);
    void addPanelToGroup(const std::string& group_name, const std::string& panel_id);
    void setGroupCollapsed(const std::string& group_name, bool collapsed);
    
    // Rendering
    void render();
    void renderLayoutGroup(const std::string& group_name);
    void renderPanel(const std::string& panel_id);
    void renderTabs();
    
    // Layout Presets
    void applyTradingLayout();
    void applyAnalysisLayout();
    void applyMinimalLayout();
    void applyDevelopmentLayout();
    void resetToDefault();
    
    // Customization
    void setGroupStyle(const std::string& group_name, ImVec4 border_color, ImVec4 bg_color);
    void setGlobalSpacing(float padding, float spacing);
    void enableCustomization(bool enable) { customization_mode_ = enable; }
    
    // Persistence
    void saveLayout(const std::string& filename);
    void loadLayout(const std::string& filename);
    
    // Viewport Management
    void updateViewport();
    ImVec2 getViewportSize() const { return viewport_size_; }
    void setRefreshInterval(float seconds) { refresh_interval_ = seconds; }
    
private:
    std::map<std::string, UIPanel> panels_;
    std::map<std::string, LayoutGroup> layout_groups_;
    ImVec2 viewport_size_;
    float global_padding_ = 8.0f;
    float global_spacing_ = 4.0f;
    bool customization_mode_ = false;
    int active_tab_ = 0;
    std::chrono::steady_clock::time_point last_render_{};
    float refresh_interval_ = 0.05f;
    
    // Helper methods
    void renderCustomizationControls();
    void renderGroupHeader(const LayoutGroup& group);
    void calculateAutoLayout();
    ImVec2 calculatePanelSize(const UIPanel& panel, const LayoutGroup& group);
    void drawGroupBorder(const LayoutGroup& group);
};

// Predefined Layout Templates
namespace LayoutTemplates {
    void setupTradingLayout(UILayoutManager& manager);
    void setupAnalysisLayout(UILayoutManager& manager);
    void setupMinimalLayout(UILayoutManager& manager);
    void setupDevelopmentLayout(UILayoutManager& manager);
}

} // namespace sep::workbench
