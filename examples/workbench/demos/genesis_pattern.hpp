#pragma once

#include "demo_manager.hpp"
#include "demos/pattern_processor.hpp"
#include "memory/quantum_coherence_manager.h"
#include <memory>

namespace sep {
namespace workbench {

class GenesisPatternDemo : public Demo {
public:
    void on_load() override;
    void on_update(float dt) override;
    void on_render() override;
    void on_unload() override;
    void on_key_press(int key) override;
    void on_mouse(int x, int y, int button) override;

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

    std::unique_ptr<PatternProcessor> pattern_processor_;
    std::unique_ptr<QuantumCoherenceManager> coherence_manager_;

    bool auto_evolve_{true};
    float evolution_rate_{0.1f};
    float coherence_threshold_{0.5f};
};

} // namespace workbench
} // namespace sep
