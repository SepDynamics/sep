#include "landing_page.hpp"
#include "workbench_core.hpp"
#include <imgui.h>
#include <imgui_internal.h>
#include <algorithm>
#include <cmath>

namespace sep::workbench {

// Helper functions for ImVec2 operations
inline ImVec2 operator+(const ImVec2& a, const ImVec2& b) {
    return ImVec2(a.x + b.x, a.y + b.y);
}

inline ImVec2 operator-(const ImVec2& a, const ImVec2& b) {
    return ImVec2(a.x - b.x, a.y - b.y);
}

LandingPage::LandingPage(WorkbenchCore* core) : core_(core) {
    initializeDemoData();
    last_update_ = std::chrono::steady_clock::now();
}

LandingPage::~LandingPage() = default;

void LandingPage::initializeDemoData() {
    // Initialize available demos
    available_demos_ = {
        {
            "genesis", "Genesis Pattern", 
            "Explore the emergence of coherent patterns from quantum fluctuations",
            "Quantum", "", false, false, {"quantum", "emergence", "visualization"}
        },
        {
            "neural", "Neural Dynamics",
            "Simulate neural network behavior with quantum coherence",
            "Neural", "", false, false, {"neural", "dynamics", "simulation"}
        },
        {
            "memory", "Memory Garden",
            "Visualize memory tier transitions and coherence evolution",
            "Memory", "", false, false, {"memory", "tiers", "coherence"}
        },
        {
            "flocking", "Flocking Behavior",
            "Emergent collective behavior from simple rules",
            "Simulation", "", false, false, {"flocking", "emergence", "collective"}
        },
        {
            "cosmo", "Cosmological Simulation",
            "N-body gravitational dynamics with quantum effects",
            "Physics", "", false, true, {"cosmology", "gravity", "physics"}
        },
        {
            "physics", "Digital Physics",
            "Cellular automata and discrete physics simulations",
            "Physics", "", false, false, {"cellular", "automata", "discrete"}
        },
        {
            "drug", "Drug Discovery",
            "Molecular docking optimization with quantum algorithms",
            "Chemistry", "", true, true, {"drug", "molecular", "optimization"}
        },
        {
            "audio", "Audio Visualizer",
            "Real-time audio analysis with pattern emergence",
            "Audio", "", false, false, {"audio", "real-time", "visualization"}
        }
    };
    
    // Extract unique categories
    categories_ = {"All"};
    for (const auto& demo : available_demos_) {
        if (std::find(categories_.begin(), categories_.end(), demo.category) == categories_.end()) {
            categories_.push_back(demo.category);
        }
    }
}

void LandingPage::render() {
    updateAnimation();
    
    // Main window
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | 
                                   ImGuiWindowFlags_NoMove |
                                   ImGuiWindowFlags_NoSavedSettings |
                                   ImGuiWindowFlags_NoBringToFrontOnFocus;
    
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin("LandingPage", nullptr, window_flags);
    ImGui::PopStyleVar();
    
    renderHeader();
    
    // Main content area
    ImGui::BeginChild("MainContent", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), false);
    
    if (config_.show_system_status) {
        renderSystemStatus();
        ImGui::Separator();
    }
    
    renderSearchBar();
    renderCategoryFilter();
    renderDemoGrid();
    
    ImGui::EndChild();
    
    renderFooter();
    
    ImGui::End();
    
    // Demo details popup
    if (!selected_demo_id_.empty()) {
        renderDemoDetails();
    }
}

void LandingPage::renderHeader() {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(20, 20));
    ImGui::BeginChild("Header", ImVec2(0, 120), true, ImGuiWindowFlags_NoScrollbar);
    
    // Title with animation
    float title_offset = std::sin(animation_time_ * 2.0f) * 5.0f;
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + title_offset);
    
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]); // Assume larger font at index 0
    ImGui::TextColored(ImVec4(0.2f, 0.8f, 1.0f, 1.0f), "SEP Workbench");
    ImGui::PopFont();
    
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "v0.1.0");
    
    ImGui::Text("Semantic Emergence Platform - Interactive Demo Suite");
    
    // Quick actions
    ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() - 200, 20));
    renderQuickActions();
    
    ImGui::EndChild();
    ImGui::PopStyleVar();
}

void LandingPage::renderSystemStatus() {
    ImGui::BeginChild("SystemStatus", ImVec2(0, 100), true);
    
    ImGui::Columns(4, "StatusColumns", true);
    
    // Service status
    ImGui::Text("Service Status");
    if (core_->isServiceConnected()) {
        ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "Connected");
        
        auto health = core_->getMetrics();
        ImGui::Text("Latency: %.1f ms", 0.0f); // Would get from service
    } else {
        ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.2f, 1.0f), "Offline Mode");
        ImGui::TextWrapped("Running with local engine");
    }
    
    ImGui::NextColumn();
    
    // GPU status
    ImGui::Text("GPU Status");
    ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "Available");
    ImGui::Text("NVIDIA RTX 3080 Ti");
    ImGui::Text("Memory: 12GB");
    
    ImGui::NextColumn();
    
    // Performance
    ImGui::Text("Performance");
    ImGui::Text("FPS: %.1f", core_->getMetrics().average_fps);
    ImGui::Text("Frame Time: %.2f ms", 1000.0f / core_->getMetrics().average_fps);
    
    ImGui::NextColumn();
    
    // Memory
    ImGui::Text("Memory Usage");
    ImGui::Text("System: 2.3 GB");
    ImGui::Text("GPU: 1.1 GB");
    ImGui::ProgressBar(0.3f, ImVec2(-1, 0));
    
    ImGui::Columns(1);
    ImGui::EndChild();
}

void LandingPage::renderSearchBar() {
    ImGui::PushItemWidth(300);
    
    char search_buffer[256];
    strcpy(search_buffer, search_query_.c_str());
    
    ImGui::Text("Search Demos:");
    ImGui::SameLine();
    
    if (ImGui::InputText("##Search", search_buffer, sizeof(search_buffer))) {
        search_query_ = search_buffer;
    }
    
    ImGui::SameLine();
    if (ImGui::Button("Clear")) {
        search_query_.clear();
    }
    
    ImGui::PopItemWidth();
}

void LandingPage::renderCategoryFilter() {
    ImGui::Text("Category:");
    ImGui::SameLine();
    
    for (const auto& category : categories_) {
        bool is_selected = (category == selected_category_);
        
        if (is_selected) {
            ImGui::PushStyleColor(ImGuiCol_Button, getCategoryColor(category));
        }
        
        if (ImGui::Button(category.c_str())) {
            selected_category_ = category;
        }
        
        if (is_selected) {
            ImGui::PopStyleColor();
        }
        
        ImGui::SameLine();
    }
    
    ImGui::NewLine();
}

void LandingPage::renderDemoGrid() {
    ImGui::BeginChild("DemoGrid", ImVec2(0, 0), false);
    
    float window_width = ImGui::GetWindowWidth();
    float card_width = config_.thumbnail_size;
    float padding = 20.0f;
    
    int cards_per_row = static_cast<int>((window_width - padding) / (card_width + padding));
    cards_per_row = std::max(1, cards_per_row);
    
    auto filtered_demos = getFilteredDemos();
    
    if (filtered_demos.empty()) {
        ImGui::SetCursorPos(ImVec2(window_width * 0.5f - 100, 100));
        ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "No demos match your search");
    } else {
        int column = 0;
        for (const auto& demo : filtered_demos) {
            if (column > 0) {
                ImGui::SameLine();
            }
            
            renderDemoCard(demo);
            
            column++;
            if (column >= cards_per_row) {
                column = 0;
            }
        }
    }
    
    ImGui::EndChild();
}

void LandingPage::renderDemoCard(const DemoInfo& demo) {
    ImGui::PushID(demo.id.c_str());
    
    // Card animation
    float anim = getCardAnimation(demo.id);
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.8f + 0.2f * anim);
    
    ImVec2 cursor_pos = ImGui::GetCursorPos();
    ImVec2 card_size(config_.thumbnail_size, config_.thumbnail_size * 1.5f);
    
    // Card background
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 p_min = ImGui::GetWindowPos() + cursor_pos;
    ImVec2 p_max = p_min + card_size;
    
    // Hover effect
    bool is_hovered = ImGui::IsMouseHoveringRect(p_min, p_max);
    
    ImU32 col_bg = is_hovered ? 
        IM_COL32(60, 60, 70, 255) : 
        IM_COL32(40, 40, 50, 255);
    
    draw_list->AddRectFilled(p_min, p_max, col_bg, 8.0f);
    
    // Category color bar
    ImVec4 cat_color = getCategoryColor(demo.category);
    draw_list->AddRectFilled(
        p_min, 
        ImVec2(p_max.x, p_min.y + 5),
        ImGui::ColorConvertFloat4ToU32(cat_color),
        8.0f, ImDrawFlags_RoundCornersTop
    );
    
    // Content
    ImGui::SetCursorPos(cursor_pos + ImVec2(10, 15));
    ImGui::BeginGroup();
    
    // Title
    ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + config_.thumbnail_size - 20);
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "%s", demo.name.c_str());
    ImGui::PopTextWrapPos();
    
    // Description
    ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + config_.thumbnail_size - 20);
    ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "%s", demo.description.c_str());
    ImGui::PopTextWrapPos();
    
    // Requirements
    ImGui::SetCursorPosY(cursor_pos.y + card_size.y - 50);
    
    if (demo.requires_service) {
        ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.2f, 1.0f), "Requires Service");
    }
    
    if (demo.requires_gpu) {
        ImGui::TextColored(ImVec4(0.2f, 0.5f, 1.0f, 1.0f), "GPU Accelerated");
    }
    
    ImGui::EndGroup();
    
    // Click detection
    if (ImGui::InvisibleButton("##card", card_size)) {
        selected_demo_id_ = demo.id;
    }
    
    // Hover tooltip
    if (is_hovered && ImGui::BeginTooltip()) {
        ImGui::Text("Click to view details and launch");
        ImGui::EndTooltip();
    }
    
    ImGui::PopStyleVar();
    ImGui::PopID();
}

void LandingPage::renderDemoDetails() {
    ImGui::OpenPopup("Demo Details");
    
    ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_FirstUseEver);
    
    if (ImGui::BeginPopupModal("Demo Details", nullptr, ImGuiWindowFlags_NoResize)) {
        // Find demo info
        auto it = std::find_if(available_demos_.begin(), available_demos_.end(),
            [this](const DemoInfo& d) { return d.id == selected_demo_id_; });
        
        if (it != available_demos_.end()) {
            const DemoInfo& demo = *it;
            
            // Header
            ImGui::TextColored(getCategoryColor(demo.category), "[%s]", demo.category.c_str());
            ImGui::SameLine();
            ImGui::Text("%s", demo.name.c_str());
            
            ImGui::Separator();
            
            // Description
            ImGui::TextWrapped("%s", demo.description.c_str());
            
            ImGui::Spacing();
            
            // Tags
            ImGui::Text("Tags:");
            ImGui::SameLine();
            for (const auto& tag : demo.tags) {
                ImGui::TextColored(ImVec4(0.5f, 0.8f, 1.0f, 1.0f), "#%s", tag.c_str());
                ImGui::SameLine();
            }
            ImGui::NewLine();
            
            // Requirements
            ImGui::Text("Requirements:");
            if (demo.requires_service) {
                bool service_ok = core_->isServiceConnected();
                ImGui::TextColored(
                    service_ok ? ImVec4(0.2f, 1.0f, 0.2f, 1.0f) : ImVec4(1.0f, 0.2f, 0.2f, 1.0f),
                    "• SEP Service: %s", service_ok ? "Connected" : "Not Connected"
                );
            }
            
            if (demo.requires_gpu) {
                ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "• GPU: Available");
            }
            
            ImGui::Separator();
            
            // Actions
            bool can_launch = !demo.requires_service || core_->isServiceConnected();
            
            if (!can_launch) {
                ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5f);
            }
            
            if (ImGui::Button("Launch Demo", ImVec2(150, 30))) {
                if (can_launch) {
                    launchDemo(demo.id);
                    ImGui::CloseCurrentPopup();
                }
            }
            
            if (!can_launch) {
                ImGui::PopStyleVar();
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Demo requires SEP service connection");
                }
            }
            
            ImGui::SameLine();
            
            if (ImGui::Button("Cancel", ImVec2(150, 30))) {
                selected_demo_id_.clear();
                ImGui::CloseCurrentPopup();
            }
        }
        
        ImGui::EndPopup();
    }
    
    // Clear selection if popup was closed
    if (!ImGui::IsPopupOpen("Demo Details")) {
        selected_demo_id_.clear();
    }
}

void LandingPage::renderQuickActions() {
    if (ImGui::Button("Settings")) {
        // Open settings
    }
    
    ImGui::SameLine();
    
    if (ImGui::Button("About")) {
        // Show about dialog
    }
}

void LandingPage::renderFooter() {
    ImGui::Separator();
    
    ImGuiWindowFlags footer_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings;
    ImGui::BeginChild("Footer", ImVec2(0, ImGui::GetFrameHeightWithSpacing()), false, footer_flags);
    
    ImGui::Text("Ready to explore quantum-inspired computing");
    
    ImGui::SameLine(ImGui::GetWindowWidth() - 200);
    ImGui::Text("© 2024 SEP Project");
    
    ImGui::EndChild();
}

std::vector<DemoInfo> LandingPage::getFilteredDemos() const {
    std::vector<DemoInfo> filtered;
    
    for (const auto& demo : available_demos_) {
        if (matchesSearch(demo) && matchesCategory(demo)) {
            filtered.push_back(demo);
        }
    }
    
    return filtered;
}

bool LandingPage::matchesSearch(const DemoInfo& demo) const {
    if (search_query_.empty()) {
        return true;
    }
    
    // Convert to lowercase for case-insensitive search
    std::string query_lower = search_query_;
    std::transform(query_lower.begin(), query_lower.end(), query_lower.begin(), ::tolower);
    
    // Check name
    std::string name_lower = demo.name;
    std::transform(name_lower.begin(), name_lower.end(), name_lower.begin(), ::tolower);
    if (name_lower.find(query_lower) != std::string::npos) {
        return true;
    }
    
    // Check description
    std::string desc_lower = demo.description;
    std::transform(desc_lower.begin(), desc_lower.end(), desc_lower.begin(), ::tolower);
    if (desc_lower.find(query_lower) != std::string::npos) {
        return true;
    }
    
    // Check tags
    for (const auto& tag : demo.tags) {
        std::string tag_lower = tag;
        std::transform(tag_lower.begin(), tag_lower.end(), tag_lower.begin(), ::tolower);
        if (tag_lower.find(query_lower) != std::string::npos) {
            return true;
        }
    }
    
    return false;
}

bool LandingPage::matchesCategory(const DemoInfo& demo) const {
    return selected_category_ == "All" || demo.category == selected_category_;
}

void LandingPage::launchDemo(const std::string& demo_id) {
    recent_demos_.push_back(demo_id);
    if (recent_demos_.size() > 5) {
        recent_demos_.erase(recent_demos_.begin());
    }
    
    core_->selectDemo(demo_id);
}

void LandingPage::updateAnimation() {
    auto now = std::chrono::steady_clock::now();
    float dt = std::chrono::duration<float>(now - last_update_).count();
    last_update_ = now;
    
    animation_time_ += dt;
}

ImVec4 LandingPage::getCategoryColor(const std::string& category) const {
    if (category == "Quantum") return ImVec4(0.5f, 0.2f, 1.0f, 1.0f);
    if (category == "Neural") return ImVec4(1.0f, 0.5f, 0.2f, 1.0f);
    if (category == "Memory") return ImVec4(0.2f, 1.0f, 0.5f, 1.0f);
    if (category == "Simulation") return ImVec4(0.2f, 0.5f, 1.0f, 1.0f);
    if (category == "Physics") return ImVec4(1.0f, 0.2f, 0.5f, 1.0f);
    if (category == "Chemistry") return ImVec4(0.5f, 1.0f, 0.2f, 1.0f);
    if (category == "Audio") return ImVec4(1.0f, 0.8f, 0.2f, 1.0f);
    return ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
}

float LandingPage::getCardAnimation(const std::string& /*demo_id*/) const {
    // Simple hover-based animation
    return 0.0f; // Could implement per-card animation state
}

} // namespace sep::workbench