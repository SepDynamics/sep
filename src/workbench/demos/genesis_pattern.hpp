#include "workbench/demos/demo_base.hpp"
#pragma once
#include "workbench/demos/demo_base.hpp"

#include "workbench/demos/demo_base.hpp"
#include <memory>

#include "demo_manager.hpp"
#include "workbench/demos/demo_base.hpp"
#include "imgui.h"
#include "memory/quantum_coherence_manager.h"
#include "quantum/processor.h"

namespace sep {
namespace workbench {


class GenesisPatternDemo : public Demo {
public:
    void on_load(sep::Engine* engine, sep::CyclesRenderer* renderer) override;
    void on_update(float dt) override;
    void on_render() override;
    void on_ui_render() override;
    void on_unload() override;
    void on_key_press(int key) override;
    void on_mouse(int x, int y, int button);

private:
    void initializePatterns();
    void evolvePatterns(float dt);
    void updateVisualization();

    struct ViewSettings {
        float rotation{0.0f};
        float zoom{1.0f};
        bool wireframe{false};
    } view_;

    struct Metrics {
        float coherence{0.f};
        std::size_t pattern_count{0};
        float evolution_rate{0.f};
        std::size_t iterations{0};
    } metrics_;

    sep::Engine* engine_{nullptr};
    sep::CyclesRenderer* renderer_{nullptr};

    std::unique_ptr<sep::quantum::Processor> pattern_processor_;
    std::unique_ptr<sep::memory::QuantumCoherenceManager> coherence_manager_;

    bool auto_evolve_{true};
    float evolution_rate_{0.1f};
    float coherence_threshold_{0.5f};
};

} // namespace workbench
} // namespace sep
