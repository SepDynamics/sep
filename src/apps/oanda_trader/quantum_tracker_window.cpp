#include "quantum_tracker_window.hpp"
#include "imgui.h"
#include <implot.h>
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace sep::apps {

QuantumTrackerWindow::QuantumTrackerWindow() {
    // Initialize stats
    stats_ = {};
}

bool QuantumTrackerWindow::initialize() {
    try {
        // Initialize quantum signal bridge
        quantum_bridge_ = std::make_unique<sep::trading::QuantumSignalBridge>();
        if (!quantum_bridge_->initialize()) {
            std::cerr << "[QuantumTracker] Failed to initialize quantum bridge" << std::endl;
            return false;
        }
        
        // Configure with runtime-adjustable thresholds
        quantum_bridge_->setConfidenceThreshold(conf_threshold_);
        quantum_bridge_->setCoherenceThreshold(coh_threshold_);
        quantum_bridge_->setStabilityThreshold(stab_threshold_);
        
        std::cout << "[QuantumTracker] Initialized successfully" << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "[QuantumTracker] Initialization error: " << e.what() << std::endl;
        return false;
    }
}

void QuantumTrackerWindow::shutdown() {
    if (quantum_bridge_) {
        quantum_bridge_->shutdown();
    }
}

void QuantumTrackerWindow::processNewMarketData(const sep::connectors::MarketData& data) {
    std::lock_guard<std::mutex> lock(data_mutex_);
    
    // Update pips tracker (from GUI.md)
    pips_tracker_.updatePips(data.mid);
    
    // Add to history
    market_history_.push_back(data);
    if (market_history_.size() > MAX_HISTORY_SIZE) {
        market_history_.pop_front();
    }
    
    // Update existing predictions
    updatePredictions(data);
    
    // Generate new prediction if we have enough history
    if (market_history_.size() >= MIN_HISTORY_FOR_SIGNAL) {
        try {
            // Convert deque to vector for quantum analysis
            std::vector<sep::connectors::MarketData> history_vector(
                market_history_.begin(), market_history_.end());
            
            // Get quantum signal
            auto signal = quantum_bridge_->analyzeMarketData(data, history_vector, {});
            
            // Store latest signal
            latest_signal_ = signal;
            has_latest_signal_ = true;
            
            // Update metric history for plotting
            confidence_history_.push_back(signal.identifiers.confidence);
            coherence_history_.push_back(signal.identifiers.coherence);
            stability_history_.push_back(signal.identifiers.stability);
            price_history_plot_.push_back(static_cast<float>(data.mid));
            timestamp_history_.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()).count());
            
            // Maintain plot history size
            if (confidence_history_.size() > MAX_PLOT_POINTS) {
                confidence_history_.pop_front();
                coherence_history_.pop_front();
                stability_history_.pop_front();
                price_history_plot_.pop_front();
                timestamp_history_.pop_front();
            }
            
            // Make prediction for ANY directional signal (to track performance)
            if (signal.action != sep::trading::QuantumTradingSignal::HOLD && 
                signal.identifiers.confidence >= 0.1f) {  // Very low threshold for tracking
                makePrediction(signal, data);
            }
            
        } catch (const std::exception& e) {
            std::cerr << "[QuantumTracker] Signal processing error: " << e.what() << std::endl;
        }
    }
    
    // Evaluate pending predictions
    evaluatePendingPredictions(data);
    
    // Update statistics
    updateStatistics();
}

void QuantumTrackerWindow::processNewMarketData(const sep::connectors::MarketData& data, 
                                               const std::string& historical_timestamp) {
    // This is the historical version - we need to calculate the proper timestamp
    // For backtesting, we calculate how many minutes ago this candle was
    static auto base_time = std::chrono::steady_clock::now();
    static int processed_count = 0;
    
    // Each historical candle represents 1 minute in the past
    // Start from 24 hours ago and work forward
    auto historical_time = base_time - std::chrono::minutes(1440 - processed_count);
    processed_count++;
    
    // Now do the same processing as regular but pass the historical timestamp to makePrediction
    // Add to history
    market_history_.push_back(data);
    if (market_history_.size() > MAX_HISTORY_SIZE) {
        market_history_.pop_front();
    }
    
    // Update existing predictions
    updatePredictions(data);
    
    // Generate new prediction if we have enough history
    if (market_history_.size() >= MIN_HISTORY_FOR_SIGNAL) {
        try {
            // Convert deque to vector for quantum analysis
            std::vector<sep::connectors::MarketData> history_vector(
                market_history_.begin(), market_history_.end());
            
            // Get quantum signal
            auto signal = quantum_bridge_->analyzeMarketData(data, history_vector, {});
            
            // Store latest signal
            latest_signal_ = signal;
            has_latest_signal_ = true;
            
            // Update metric history for plotting
            confidence_history_.push_back(signal.identifiers.confidence);
            coherence_history_.push_back(signal.identifiers.coherence);
            stability_history_.push_back(signal.identifiers.stability);
            price_history_plot_.push_back(static_cast<float>(data.mid));
            timestamp_history_.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(
                historical_time.time_since_epoch()).count());
            
            // Maintain plot history size
            if (confidence_history_.size() > MAX_PLOT_POINTS) {
                confidence_history_.pop_front();
                coherence_history_.pop_front();
                stability_history_.pop_front();
                price_history_plot_.pop_front();
                timestamp_history_.pop_front();
            }
            
            // Make prediction for ANY directional signal (to track performance) WITH historical timestamp
            if (signal.action != sep::trading::QuantumTradingSignal::HOLD && 
                signal.identifiers.confidence >= 0.1f) {  // Very low threshold for tracking
                makePrediction(signal, data, historical_time);
            }
            
        } catch (const std::exception& e) {
            std::cerr << "[QuantumTracker] Signal processing error: " << e.what() << std::endl;
        }
    }
    
    // Update statistics
    updateStatistics();
}

void QuantumTrackerWindow::makePrediction(const sep::trading::QuantumTradingSignal& signal, 
                                          const sep::connectors::MarketData& current_data,
                                          std::optional<std::chrono::steady_clock::time_point> historical_time) {
    std::lock_guard<std::mutex> lock(predictions_mutex_);
    
    QuantumPrediction pred;
    pred.timestamp = historical_time.value_or(std::chrono::steady_clock::now());
    pred.instrument = signal.instrument;
    pred.predicted_direction = signal.action;
    pred.prediction_price = current_data.mid;
    pred.confidence = signal.identifiers.confidence;
    pred.coherence = signal.identifiers.coherence;
    pred.stability = signal.identifiers.stability;
    pred.evaluation_period = std::chrono::seconds(60); // 1 minute
    
    predictions_.push_back(pred);
    
    // Keep only last 1000 predictions
    if (predictions_.size() > 1000) {
        predictions_.erase(predictions_.begin(), predictions_.begin() + 100);
    }
    
    std::cout << "[QuantumTracker] New prediction: " << actionToString(signal.action)
              << " " << signal.instrument << " @ " << current_data.mid
              << " (confidence: " << signal.identifiers.confidence << ")" << std::endl;
}

void QuantumTrackerWindow::updatePredictions(const sep::connectors::MarketData& current_data) {
    // This function evaluates existing predictions with new market data
    evaluatePendingPredictions(current_data);
}

void QuantumTrackerWindow::evaluatePendingPredictions(const sep::connectors::MarketData& current_data) {
    std::lock_guard<std::mutex> lock(predictions_mutex_);
    
    auto now = std::chrono::steady_clock::now();
    
    for (auto& pred : predictions_) {
        if (!pred.resolved && pred.instrument == current_data.instrument) {
            // Check if evaluation period has passed
            auto elapsed = now - pred.timestamp;
            if (elapsed >= pred.evaluation_period) {
                // Evaluate prediction
                pred.actual_price_after_period = current_data.mid;
                pred.resolved = true;
                
                // Check if prediction was correct
                double price_change = current_data.mid - pred.prediction_price;
                bool price_went_up = price_change > 0.0001; // Small threshold for forex
                bool price_went_down = price_change < -0.0001;
                
                if (pred.predicted_direction == sep::trading::QuantumTradingSignal::BUY && price_went_up) {
                pred.correct = true;
                } else if (pred.predicted_direction == sep::trading::QuantumTradingSignal::SELL && price_went_down) {
                pred.correct = true;
                } else {
                    pred.correct = false;
                }
                
                std::cout << "[QuantumTracker] Prediction resolved: " 
                          << (pred.correct ? "CORRECT" : "INCORRECT")
                          << " (" << actionToString(pred.predicted_direction) << " "
                          << pred.instrument << ", change: " << price_change << ")" << std::endl;
            }
        }
    }
}

void QuantumTrackerWindow::updateStatistics() {
    std::lock_guard<std::mutex> lock(predictions_mutex_);
    
    // Reset stats
    stats_ = {};
    
    // Count predictions by status
    int resolved_count = 0;
    double confidence_sum = 0.0;
    double coherence_sum = 0.0;
    double stability_sum = 0.0;
    
    auto now = std::chrono::steady_clock::now();
    auto one_hour_ago = now - std::chrono::hours(1);
    auto one_day_ago = now - std::chrono::hours(24);
    
    int hour_correct = 0, hour_total = 0;
    int day_correct = 0, day_total = 0;
    int overall_correct = 0, overall_total = 0;
    
    for (const auto& pred : predictions_) {
        stats_.total_predictions++;
        confidence_sum += pred.confidence;
        coherence_sum += pred.coherence;
        stability_sum += pred.stability;
        
        if (pred.resolved) {
            resolved_count++;
            overall_total++;
            if (pred.correct) {
                stats_.correct_predictions++;
                overall_correct++;
            } else {
                stats_.incorrect_predictions++;
            }
            
            // Time-based accuracy (overlapping windows)
            if (pred.timestamp >= one_hour_ago) {
                hour_total++;
                if (pred.correct) hour_correct++;
            }
            
            if (pred.timestamp >= one_day_ago) {
                // Count in 24h window (includes last hour)
                day_total++;
                if (pred.correct) day_correct++;
            }
            
            // Confidence bucket tracking
            if (pred.confidence >= HIGH_CONFIDENCE_THRESHOLD) {
                stats_.high_confidence_total++;
                if (pred.correct) stats_.high_confidence_correct++;
            } else if (pred.confidence >= MEDIUM_CONFIDENCE_THRESHOLD) {
                stats_.medium_confidence_total++;
                if (pred.correct) stats_.medium_confidence_correct++;
            } else {
                stats_.low_confidence_total++;
                if (pred.correct) stats_.low_confidence_correct++;
            }
        } else {
            stats_.pending_predictions++;
        }
    }
    
    // Calculate percentages
    if (resolved_count > 0) {
        stats_.accuracy_percentage = (double)stats_.correct_predictions / resolved_count * 100.0;
    }
    
    if (hour_total > 0) {
        stats_.last_hour_accuracy = (double)hour_correct / hour_total * 100.0;
    }
    
    if (day_total > 0) {
        stats_.last_24h_accuracy = (double)day_correct / day_total * 100.0;
    }
    
    if (overall_total > 0) {
        stats_.overall_accuracy = (double)overall_correct / overall_total * 100.0;
    }
    
    if (stats_.total_predictions > 0) {
        stats_.average_confidence = confidence_sum / stats_.total_predictions;
        stats_.average_coherence = coherence_sum / stats_.total_predictions;
        stats_.average_stability = stability_sum / stats_.total_predictions;
    }
}

void QuantumTrackerWindow::render() {
    ImGui::Begin("🔮 Quantum Signal Tracker - Live Performance", nullptr, 
                 ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
    
    renderPredictionStats();
    ImGui::Separator();
    
    // New GUI.md requirements
    renderPipsDisplay();
    ImGui::Separator();
    
    renderQuantumDiagnostics();
    ImGui::Separator();

    renderThresholdControls();
    ImGui::Separator();
    
    renderMetricPlots();
    ImGui::Separator();
    
    renderLatestSignal();
    ImGui::Separator();
    
    renderMultiTimeframeConfirmation();
    ImGui::Separator();
    
    renderLiveTradingPerformance();
    ImGui::Separator();
    
    renderAccuracyMetrics();
    ImGui::Separator();
    
    renderConfidenceBuckets();
    ImGui::Separator();
    
    renderRecentPredictions();
    
    ImGui::End();
}

void QuantumTrackerWindow::renderPredictionStats() {
    ImGui::Text("📊 PREDICTION STATISTICS");
    
    // Main stats in colored boxes
    ImGui::BeginGroup();
    
    // Total predictions
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.3f, 0.8f, 0.6f));
    ImGui::Button("Total", ImVec2(80, 40));
    ImGui::PopStyleColor();
    ImGui::SameLine();
    ImGui::Text("%d", stats_.total_predictions);
    
    ImGui::SameLine(150);
    
    // Correct predictions (green)
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.8f, 0.2f, 0.6f));
    ImGui::Button("Correct", ImVec2(80, 40));
    ImGui::PopStyleColor();
    ImGui::SameLine();
    ImGui::Text("%d", stats_.correct_predictions);
    
    ImGui::SameLine(300);
    
    // Incorrect predictions (red)
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.2f, 0.2f, 0.6f));
    ImGui::Button("Wrong", ImVec2(80, 40));
    ImGui::PopStyleColor();
    ImGui::SameLine();
    ImGui::Text("%d", stats_.incorrect_predictions);
    
    ImGui::SameLine(450);
    
    // Pending predictions (yellow)
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.8f, 0.2f, 0.6f));
    ImGui::Button("Pending", ImVec2(80, 40));
    ImGui::PopStyleColor();
    ImGui::SameLine();
    ImGui::Text("%d", stats_.pending_predictions);
    
    ImGui::EndGroup();
    
    // Overall accuracy - big number
    ImGui::Spacing();
    // ImGui::PushFont(nullptr); // Could use larger font if available
    if (stats_.accuracy_percentage >= 60.0) {
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "ACCURACY: %.1f%%", stats_.accuracy_percentage);
    } else if (stats_.accuracy_percentage >= 50.0) {
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "ACCURACY: %.1f%%", stats_.accuracy_percentage);
    } else {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "ACCURACY: %.1f%%", stats_.accuracy_percentage);
    }
    // ImGui::PopFont();
}

void QuantumTrackerWindow::renderLatestSignal() {
    ImGui::Text("🔬 LATEST QUANTUM SIGNAL");
    
    if (!has_latest_signal_) {
        ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "Waiting for signal...");
        return;
    }
    
    // Signal action with color coding
    const char* action_str = actionToString(latest_signal_.action);
    ImVec4 action_color;
    if (latest_signal_.action == sep::trading::QuantumTradingSignal::BUY) {
        action_color = ImVec4(0.0f, 1.0f, 0.0f, 1.0f); // Green
    } else if (latest_signal_.action == sep::trading::QuantumTradingSignal::SELL) {
        action_color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f); // Red
    } else {
        action_color = ImVec4(0.7f, 0.7f, 0.7f, 1.0f); // Gray
    }
    
    ImGui::Text("Direction: ");
    ImGui::SameLine();
    ImGui::TextColored(action_color, "%s", action_str);
    ImGui::SameLine();
    ImGui::Text("(%s)", latest_signal_.instrument.c_str());
    
    // Quantum metrics with progress bars
    ImGui::Text("Confidence: %.3f", latest_signal_.identifiers.confidence);
    ImGui::SameLine(150);
    ImGui::ProgressBar(latest_signal_.identifiers.confidence, ImVec2(200, 0));
    
    ImGui::Text("Coherence:  %.3f", latest_signal_.identifiers.coherence);
    ImGui::SameLine(150);
    ImGui::ProgressBar(latest_signal_.identifiers.coherence, ImVec2(200, 0));
    
    ImGui::Text("Stability:  %.3f", latest_signal_.identifiers.stability);
    ImGui::SameLine(150);
    ImGui::ProgressBar(std::max(0.0f, latest_signal_.identifiers.stability), ImVec2(200, 0));
    
    // Execute signal status
    if (latest_signal_.should_execute) {
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "⚡ SIGNAL ACTIVE");
    } else {
        ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "○ Signal below threshold");
    }
}

void QuantumTrackerWindow::renderAccuracyMetrics() {
    ImGui::Text("📈 TIME-BASED ACCURACY");
    
    ImGui::Text("Last Hour:  %.1f%%", stats_.last_hour_accuracy);
    ImGui::SameLine(150);
    ImGui::ProgressBar(stats_.last_hour_accuracy / 100.0f, ImVec2(200, 0));
    
    ImGui::Text("Last 24h:   %.1f%%", stats_.last_24h_accuracy);
    ImGui::SameLine(150);
    ImGui::ProgressBar(stats_.last_24h_accuracy / 100.0f, ImVec2(200, 0));
    
    ImGui::Text("Overall:    %.1f%%", stats_.accuracy_percentage);
    ImGui::SameLine(150);
    ImGui::ProgressBar(stats_.accuracy_percentage / 100.0f, ImVec2(200, 0));
    
    // Average quantum metrics
    ImGui::Spacing();
    ImGui::Text("Avg Confidence: %.3f", stats_.average_confidence);
    ImGui::Text("Avg Coherence:  %.3f", stats_.average_coherence);
    ImGui::Text("Avg Stability:  %.3f", stats_.average_stability);
}

void QuantumTrackerWindow::renderConfidenceBuckets() {
    ImGui::Text("🎯 CONFIDENCE ANALYSIS");
    
    // High confidence
    if (stats_.high_confidence_total > 0) {
        double high_acc = (double)stats_.high_confidence_correct / stats_.high_confidence_total * 100.0;
        ImGui::Text("High (≥80%%): %d/%d (%.1f%%)", 
                   stats_.high_confidence_correct, stats_.high_confidence_total, high_acc);
    } else {
        ImGui::Text("High (≥80%%): 0/0 (N/A)");
    }
    
    // Medium confidence
    if (stats_.medium_confidence_total > 0) {
        double med_acc = (double)stats_.medium_confidence_correct / stats_.medium_confidence_total * 100.0;
        ImGui::Text("Med (60-80%%): %d/%d (%.1f%%)", 
                   stats_.medium_confidence_correct, stats_.medium_confidence_total, med_acc);
    } else {
        ImGui::Text("Med (60-80%%): 0/0 (N/A)");
    }
    
    // Low confidence
    if (stats_.low_confidence_total > 0) {
        double low_acc = (double)stats_.low_confidence_correct / stats_.low_confidence_total * 100.0;
        ImGui::Text("Low (<60%%):  %d/%d (%.1f%%)", 
                   stats_.low_confidence_correct, stats_.low_confidence_total, low_acc);
    } else {
        ImGui::Text("Low (<60%%):  0/0 (N/A)");
    }
}

void QuantumTrackerWindow::renderRecentPredictions() {
    ImGui::Text("📋 RECENT PREDICTIONS");
    
    if (ImGui::BeginTable("predictions", 6, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
        ImGui::TableSetupColumn("Time");
        ImGui::TableSetupColumn("Direction");
        ImGui::TableSetupColumn("Confidence");
        ImGui::TableSetupColumn("Status");
        ImGui::TableSetupColumn("Result");
        ImGui::TableSetupColumn("Duration");
        ImGui::TableHeadersRow();
        
        std::lock_guard<std::mutex> lock(predictions_mutex_);
        
        // Show last 10 predictions
        int start_idx = std::max(0, (int)predictions_.size() - 10);
        for (int i = predictions_.size() - 1; i >= start_idx; --i) {
            const auto& pred = predictions_[i];
            
            ImGui::TableNextRow();
            
            // Time
            ImGui::TableNextColumn();
            ImGui::Text("%s", formatDuration(pred.timestamp).c_str());
            
            // Direction
            ImGui::TableNextColumn();
            ImVec4 color = (pred.predicted_direction == sep::trading::QuantumTradingSignal::BUY) ?
                          ImVec4(0.0f, 1.0f, 0.0f, 1.0f) : ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
            ImGui::TextColored(color, "%s", actionToString(pred.predicted_direction));
            
            // Confidence
            ImGui::TableNextColumn();
            ImGui::Text("%.2f", pred.confidence);
            
            // Status
            ImGui::TableNextColumn();
            if (pred.resolved) {
                ImGui::Text("Done");
            } else {
                ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Pending");
            }
            
            // Result
            ImGui::TableNextColumn();
            if (pred.resolved) {
                if (pred.correct) {
                    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "✓");
                } else {
                    ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "✗");
                }
            } else {
                ImGui::Text("-");
            }
            
            // Duration
            ImGui::TableNextColumn();
            if (pred.resolved) {
                ImGui::Text("60s");
            } else {
                auto elapsed = std::chrono::steady_clock::now() - pred.timestamp;
                auto seconds = std::chrono::duration_cast<std::chrono::seconds>(elapsed).count();
                ImGui::Text("%lds", seconds);
            }
        }
        
        ImGui::EndTable();
    }
}

void QuantumTrackerWindow::resetStats() {
    std::lock_guard<std::mutex> lock(predictions_mutex_);
    predictions_.clear();
    stats_ = {};
}

std::string QuantumTrackerWindow::formatDuration(std::chrono::steady_clock::time_point start) const {
    auto now = std::chrono::steady_clock::now();
    auto elapsed = now - start;
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(elapsed).count();
    
    if (seconds < 60) {
        return std::to_string(seconds) + "s ago";
    } else if (seconds < 3600) {
        return std::to_string(seconds / 60) + "m ago";
    } else {
        return std::to_string(seconds / 3600) + "h ago";
    }
}

const char* QuantumTrackerWindow::actionToString(sep::trading::QuantumTradingSignal::Action action) const {
    switch (action) {
        case sep::trading::QuantumTradingSignal::BUY: return "BUY";
        case sep::trading::QuantumTradingSignal::SELL: return "SELL";
        case sep::trading::QuantumTradingSignal::HOLD: return "HOLD";
        default: return "UNKNOWN";
    }
}

// New GUI.md requirements implementation
void QuantumTrackerWindow::renderPipsDisplay() {
    ImGui::Begin("📈 Live Pips Tracking (48h Window)");
    
    ImGui::Text("Current Price: %.5f", pips_tracker_.current_price_);
    ImGui::Text("48h Start Price: %.5f", pips_tracker_.start_price_48h_);
    
    // Color-coded pips display
    if (pips_tracker_.total_pips_48h_ > 0) {
        ImGui::TextColored(ImVec4(0, 1, 0, 1), "Total Pips (48h): +%.2f", pips_tracker_.total_pips_48h_);
    } else {
        ImGui::TextColored(ImVec4(1, 0, 0, 1), "Total Pips (48h): %.2f", pips_tracker_.total_pips_48h_);
    }
    
    ImGui::Text("Data Points: %zu / 2880 (48h)", pips_tracker_.price_history_.size());
    ImGui::Text("Window Complete: %s", pips_tracker_.price_history_.size() >= 2880 ? "YES" : "NO");

    // Advanced performance metrics
    ImGui::Text("Sharpe Ratio: %.2f", pips_tracker_.calculateSharpeRatio());
    ImGui::Text("Max Drawdown: %.2f%%", pips_tracker_.calculateMaxDrawdown() * 100.0);
    
    ImGui::End();
}

void QuantumTrackerWindow::renderQuantumDiagnostics() {
    ImGui::Begin("🔬 Quantum Engine Diagnostics");
    
    if (has_latest_signal_) {
        ImGui::Text("🔍 Raw Quantum Metrics:");
        ImGui::Text("  Confidence: %.3f (threshold: %.1f)", latest_signal_.identifiers.confidence, 0.6f);
        ImGui::Text("  Coherence: %.3f (threshold: %.1f)", latest_signal_.identifiers.coherence, 0.4f);
        ImGui::Text("  Stability: %.3f (threshold: %.1f)", latest_signal_.identifiers.stability, 0.0f);
        
        ImGui::Separator();
        ImGui::Text("🧬 QFH Analysis:");
        ImGui::Text("  Flip Ratio: %.3f", latest_signal_.identifiers.flip_ratio);
        ImGui::Text("  Rupture Ratio: %.3f", latest_signal_.identifiers.rupture_ratio);
        ImGui::Text("  Entropy: %.3f", latest_signal_.identifiers.entropy);
        ImGui::Text("  Collapse Detected: %s", latest_signal_.identifiers.quantum_collapse_detected ? "YES" : "NO");
        
        ImGui::Separator();
        ImGui::Text("📊 Threshold Analysis:");
        bool conf_pass = latest_signal_.identifiers.confidence >= 0.6f;
        bool coh_pass = latest_signal_.identifiers.coherence >= 0.4f;
        bool stab_pass = latest_signal_.identifiers.stability >= 0.0f;
        
        ImGui::TextColored(conf_pass ? ImVec4(0,1,0,1) : ImVec4(1,0,0,1), 
                          "Confidence: %s", conf_pass ? "PASS" : "FAIL");
        ImGui::TextColored(coh_pass ? ImVec4(0,1,0,1) : ImVec4(1,0,0,1), 
                          "Coherence: %s", coh_pass ? "PASS" : "FAIL");
        ImGui::TextColored(stab_pass ? ImVec4(0,1,0,1) : ImVec4(1,0,0,1), 
                          "Stability: %s", stab_pass ? "PASS" : "FAIL");
    } else {
        ImGui::Text("Waiting for quantum signal...");
    }
    
    ImGui::End();
}

void QuantumTrackerWindow::renderThresholdControls() {
    ImGui::Begin("🎛 Threshold Controls");

    bool updated = false;
    updated |= ImGui::SliderFloat("Confidence", &conf_threshold_, 0.0f, 1.0f);
    updated |= ImGui::SliderFloat("Coherence", &coh_threshold_, 0.0f, 1.0f);
    updated |= ImGui::SliderFloat("Stability", &stab_threshold_, -1.0f, 1.0f);

    if (updated && quantum_bridge_) {
        quantum_bridge_->setConfidenceThreshold(conf_threshold_);
        quantum_bridge_->setCoherenceThreshold(coh_threshold_);
        quantum_bridge_->setStabilityThreshold(stab_threshold_);
    }

    ImGui::End();
}

void QuantumTrackerWindow::renderMetricPlots() {
    if (confidence_history_.empty() || timestamp_history_.empty()) {
        ImGui::Text("No data to plot yet...");
        return; // No data to plot yet
    }
    
    // Ensure all data vectors are the same size
    size_t min_size = std::min({
        confidence_history_.size(),
        coherence_history_.size(), 
        stability_history_.size(),
        price_history_plot_.size(),
        timestamp_history_.size()
    });
    
    if (min_size == 0) {
        ImGui::Text("Waiting for data...");
        return;
    }
    
    // Create time axis for plotting (use float to match metric data)
    std::vector<float> time_axis;
    time_axis.reserve(min_size);
    
    // Use first timestamp ever recorded, not rolling window, to prevent chart compression
    static double first_timestamp = 0.0;
    static bool first_timestamp_set = false;
    if (!first_timestamp_set) {
        first_timestamp = timestamp_history_.front();
        first_timestamp_set = true;
    }
    
    for (size_t i = 0; i < min_size; ++i) {
        // Convert milliseconds to seconds (timestamp_history_ is in milliseconds)
        time_axis.push_back(static_cast<float>((timestamp_history_[i] - first_timestamp) / 1000.0));
    }
    
    if (ImPlot::BeginPlot("Quantum Metrics Over Time", ImVec2(-1, 300))) {
        ImPlot::SetupAxes("Time (seconds)", "Value");
        ImPlot::SetupAxisLimits(ImAxis_Y1, 0.0, 1.0, ImGuiCond_Always);
        
        // Convert deques to vectors for plotting with size limitation
        std::vector<float> confidence_vec;
        std::vector<float> coherence_vec;
        std::vector<float> stability_vec;
        
        confidence_vec.reserve(min_size);
        coherence_vec.reserve(min_size);
        stability_vec.reserve(min_size);
        
        auto conf_it = confidence_history_.begin();
        auto coh_it = coherence_history_.begin();
        auto stab_it = stability_history_.begin();
        
        for (size_t i = 0; i < min_size; ++i) {
            confidence_vec.push_back(*conf_it++);
            coherence_vec.push_back(*coh_it++);
            stability_vec.push_back(*stab_it++);
        }
        
        // Only plot if we have valid data
        if (!confidence_vec.empty() && confidence_vec.size() == time_axis.size()) {
            // Plot confidence
            ImPlot::SetNextLineStyle(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), 2.0f); // Red
            ImPlot::PlotLine("Confidence", time_axis.data(), confidence_vec.data(), static_cast<int>(min_size));
            
            // Plot coherence  
            ImPlot::SetNextLineStyle(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), 2.0f); // Green
            ImPlot::PlotLine("Coherence", time_axis.data(), coherence_vec.data(), static_cast<int>(min_size));
            
            // Plot stability
            ImPlot::SetNextLineStyle(ImVec4(0.0f, 0.0f, 1.0f, 1.0f), 2.0f); // Blue
            ImPlot::PlotLine("Stability", time_axis.data(), stability_vec.data(), static_cast<int>(min_size));
            
            // Add threshold lines only if we have data
            if (time_axis.size() >= 2) {
                ImPlot::SetNextLineStyle(ImVec4(1.0f, 0.0f, 0.0f, 0.5f), 1.0f); // Red dashed
                float conf_threshold = 0.8f;
                std::vector<float> conf_thresh_line(time_axis.size(), conf_threshold);
                ImPlot::PlotLine("Conf Threshold", time_axis.data(), conf_thresh_line.data(), static_cast<int>(time_axis.size()));
                
                ImPlot::SetNextLineStyle(ImVec4(0.0f, 1.0f, 0.0f, 0.5f), 1.0f); // Green dashed
                float coh_threshold = 0.7f;
                std::vector<float> coh_thresh_line(time_axis.size(), coh_threshold);
                ImPlot::PlotLine("Coh Threshold", time_axis.data(), coh_thresh_line.data(), static_cast<int>(time_axis.size()));
            }
        }
        
        ImPlot::EndPlot();
    }
    
    // Price plot
    if (ImPlot::BeginPlot("Price Movement", ImVec2(-1, 200))) {
        ImPlot::SetupAxes("Time (seconds)", "Price");
        
        if (min_size > 0 && time_axis.size() == min_size) {
            std::vector<float> price_vec;
            price_vec.reserve(min_size);
            
            auto price_it = price_history_plot_.begin();
            for (size_t i = 0; i < min_size; ++i) {
                price_vec.push_back(*price_it++);
            }
            
            if (!price_vec.empty()) {
                ImPlot::SetNextLineStyle(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), 2.0f); // Yellow
                ImPlot::PlotLine("EUR/USD", time_axis.data(), price_vec.data(), static_cast<int>(min_size));
            }
        }
        
        ImPlot::EndPlot();
    }
}

void QuantumTrackerWindow::renderMultiTimeframeConfirmation() {
    ImGui::Begin("🕒 Multi-Timeframe Confirmation");

    if (!has_latest_signal_) {
        ImGui::Text("Waiting for signal...");
        ImGui::End();
        return;
    }

    auto render_signal_status = [](const char* label, sep::trading::QuantumTradingSignal::Action action) {
        ImGui::Text("%s:", label);
        ImGui::SameLine(50);
        if (action == sep::trading::QuantumTradingSignal::BUY) {
            ImGui::TextColored(ImVec4(0, 1, 0, 1), "BUY");
        } else if (action == sep::trading::QuantumTradingSignal::SELL) {
            ImGui::TextColored(ImVec4(1, 0, 0, 1), "SELL");
        } else {
            ImGui::TextDisabled("HOLD");
        }
    };

    // Display the timeframe statuses
    render_signal_status("M1 ", latest_signal_.action);
    render_signal_status("M5 ", latest_signal_.mtf_confirmation.m5_confirms ? latest_signal_.action : sep::trading::QuantumTradingSignal::HOLD);
    render_signal_status("M15", latest_signal_.mtf_confirmation.m15_confirms ? latest_signal_.action : sep::trading::QuantumTradingSignal::HOLD);

    ImGui::Separator();

    // Display confirmation status
    if (latest_signal_.mtf_confirmation.triple_confirmed) {
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.8f, 1.0f), "✅ TRIPLE CONFIRMED");
    } else {
        ImGui::TextDisabled("⏳ Awaiting Confirmation...");
    }

    // Display confidence levels
    ImGui::Separator();
    ImGui::Text("Confidence Levels:");
    ImGui::Text("M5:  %.2f", latest_signal_.mtf_confirmation.m5_confidence);
    ImGui::Text("M15: %.2f", latest_signal_.mtf_confirmation.m15_confidence);

    ImGui::End();
}

void QuantumTrackerWindow::renderLiveTradingPerformance() {
    ImGui::Begin("📈 Live Trading Performance");
    
    const auto& stats = getStats();
    
    // P/L Summary
    ImGui::Text("💰 PROFIT & LOSS");
    ImGui::Separator();
    
    // Total P/L with color coding
    if (stats.total_pnl > 0) {
        ImGui::TextColored(ImVec4(0, 1, 0, 1), "Total P/L: +$%.2f", stats.total_pnl);
    } else if (stats.total_pnl < 0) {
        ImGui::TextColored(ImVec4(1, 0, 0, 1), "Total P/L: -$%.2f", std::abs(stats.total_pnl));
    } else {
        ImGui::Text("Total P/L: $0.00");
    }
    
    // Trade Statistics
    ImGui::Spacing();
    ImGui::Text("📊 TRADE STATISTICS");
    ImGui::Separator();
    
    ImGui::Text("Total Trades: %d", stats.trades_executed);
    ImGui::Text("Winning Trades: %d", stats.winning_trades);
    ImGui::Text("Losing Trades: %d", stats.losing_trades);
    
    // Win Rate with color coding
    if (stats.win_rate >= 60.0) {
        ImGui::TextColored(ImVec4(0, 1, 0, 1), "Win Rate: %.1f%% ✅", stats.win_rate);
    } else if (stats.win_rate >= 50.0) {
        ImGui::TextColored(ImVec4(1, 1, 0, 1), "Win Rate: %.1f%% ⚠️", stats.win_rate);
    } else {
        ImGui::TextColored(ImVec4(1, 0, 0, 1), "Win Rate: %.1f%% ❌", stats.win_rate);
    }
    
    // Risk Management
    ImGui::Spacing();
    ImGui::Text("⚠️ RISK MANAGEMENT");
    ImGui::Separator();
    
    // Max Drawdown with warning colors
    if (stats.max_drawdown > 0.15) { // 15% warning
        ImGui::TextColored(ImVec4(1, 0, 0, 1), "Max Drawdown: %.1f%% ⚠️", stats.max_drawdown * 100);
    } else if (stats.max_drawdown > 0.10) { // 10% caution
        ImGui::TextColored(ImVec4(1, 1, 0, 1), "Max Drawdown: %.1f%%", stats.max_drawdown * 100);
    } else {
        ImGui::TextColored(ImVec4(0, 1, 0, 1), "Max Drawdown: %.1f%%", stats.max_drawdown * 100);
    }
    
    ImGui::Text("Current Drawdown: %.1f%%", stats.current_drawdown * 100);
    
    // Performance vs Backtest
    ImGui::Spacing();
    ImGui::Text("🎯 VS BACKTEST TARGET");
    ImGui::Separator();
    
    ImGui::Text("Target Win Rate: 60.0%%");
    if (stats.trades_executed > 0) {
        float performance_ratio = stats.win_rate / 60.0f;
        if (performance_ratio >= 0.95f) {
            ImGui::TextColored(ImVec4(0, 1, 0, 1), "Performance: %.1f%% of target ✅", performance_ratio * 100);
        } else if (performance_ratio >= 0.80f) {
            ImGui::TextColored(ImVec4(1, 1, 0, 1), "Performance: %.1f%% of target ⚠️", performance_ratio * 100);
        } else {
            ImGui::TextColored(ImVec4(1, 0, 0, 1), "Performance: %.1f%% of target ❌", performance_ratio * 100);
        }
    } else {
        ImGui::TextDisabled("No trades executed yet");
    }
    
    ImGui::End();
}

} // namespace sep::apps
