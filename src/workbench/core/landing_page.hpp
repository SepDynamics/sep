#pragma once

#include <string>
#include <vector>
#include "imgui.h"
#include <memory>
#include <functional>
#include <chrono>

namespace sep::workbench {

// Forward declarations
class WorkbenchCore;

// Demo information structure
struct DemoInfo {
    std::string id;
    std::string name;
    std::string description;
    std::string category;
    std::string thumbnail_path;
    bool requires_service;
    bool requires_gpu;
    std::vector<std::string> tags;
};

// Landing page configuration
struct LandingPageConfig {
    bool show_system_status{true};
    bool show_recent_demos{true};
    bool enable_search{true};
    bool enable_categories{true};
    float thumbnail_size{200.0f};
    int demos_per_row{4};
};

class LandingPage {
public:
    explicit LandingPage(WorkbenchCore* core);
    ~LandingPage();

    // Main render function
    void render();

    // UI state management
    void setSelectedDemo(const std::string& demo_id) { selected_demo_id_ = demo_id; }
    std::string getSelectedDemo() const { return selected_demo_id_; }

    // Configuration
    void setConfig(const LandingPageConfig& config) { config_ = config; }
    const LandingPageConfig& getConfig() const { return config_; }

private:
    // Core reference
    WorkbenchCore* core_{nullptr};
    
    // Configuration
    LandingPageConfig config_;
    
    // UI State
    std::string selected_demo_id_;
    std::string search_query_;
    std::string selected_category_{"All"};
    std::vector<std::string> recent_demos_;
    
    // Demo data
    std::vector<DemoInfo> available_demos_;
    std::vector<std::string> categories_;
    
    // Animation state
    float animation_time_{0.0f};
    std::chrono::steady_clock::time_point last_update_;
    
    // Render components
    void renderHeader();
    void renderSystemStatus();
    void renderDemoGrid();
    void renderDemoCard(const DemoInfo& demo);
    void renderSearchBar();
    void renderCategoryFilter();
    void renderDemoDetails();
    void renderQuickActions();
    void renderFooter();
    
    // Helper functions
    void initializeDemoData();
    std::vector<DemoInfo> getFilteredDemos() const;
    bool matchesSearch(const DemoInfo& demo) const;
    bool matchesCategory(const DemoInfo& demo) const;
    void launchDemo(const std::string& demo_id);
    void updateAnimation();
    
    // UI helpers
    float getCardAnimation(const std::string& demo_id) const;
    ImVec4 getCategoryColor(const std::string& category) const;
};

} // namespace sep::workbench