#pragma once

#include "metrics_monitor.h"
#include "memory_monitor.hpp"
#include "file_dialog.hpp"
#include "oanda_connector.h"
#include <memory>
#include <vector>
#include <string>

namespace sep::workbench {

/**
 * @brief ImGui-based dashboard for real-time SEP metrics visualization
 * 
 * Provides an investor-ready interface showing:
 * - Real-time coherence, stability, entropy graphs
 * - Pattern detection statistics
 * - Data source selection and control
 * - Export and analysis tools
 */
class MetricsDashboard {
public:
    MetricsDashboard();
    ~MetricsDashboard();

    bool initialize();
    void shutdown();
    void render();  // Call this in ImGui render loop

    // Dashboard state
    bool isVisible() const { return show_dashboard_; }
    void setVisible(bool visible) { show_dashboard_ = visible; }

private:
    void renderControlPanel();
    void renderSystemMetrics();
    void renderPatternList();
    void renderMetricsGraphs();
    void renderDataSourceSelector();
    void renderMemoryMonitor();
    
    // Control panel actions
    void handleDataLoad();
    void handleClearData();
    void handleExportMetrics();
    void handleStartProcessing();
    void handleStopProcessing();

    std::unique_ptr<MetricsMonitor> monitor_;
    std::unique_ptr<MemoryMonitor> memory_monitor_;
    FileDialog file_dialog_;
    
    // UI State
    bool show_dashboard_{true};
    bool show_control_panel_{true};
    bool show_system_metrics_{true};
    bool show_pattern_list_{true};
    bool show_metrics_graphs_{true};
    bool show_memory_monitor_{true};
    
    // Data source selection
    char file_path_buffer_[512] = "";
    int data_source_type_{0};  // 0=File, 1=Live Stream, 2=Generated
    
    // Graph data for real-time plotting
    std::vector<float> coherence_history_;
    std::vector<float> stability_history_;
    std::vector<float> entropy_history_;
    size_t max_history_size_{1000};
    
    // Pattern filtering
    float min_coherence_filter_{0.0f};
    float min_stability_filter_{0.0f};
    float max_entropy_filter_{1.0f};
    int pattern_sort_mode_{0};  // 0=Coherence, 1=Stability, 2=Frequency
    
    // Export settings
    char export_path_buffer_[512] = "metrics_export.json";
    
    // Memory monitoring
    std::vector<float> memory_history_;
    std::vector<float> memory_growth_history_;
    bool auto_monitor_memory_{true};
    
    // OANDA integration
    std::unique_ptr<sep::connectors::OandaConnector> oanda_connector_;
    bool oanda_connected_{false};
    std::string oanda_status_{"Disconnected"};
    bool use_oanda_data_{false};
    
    // OANDA helper methods
    void initializeOandaConnection();
    void updateOandaData();
    void renderOandaPanel();
};

} // namespace sep::workbench
