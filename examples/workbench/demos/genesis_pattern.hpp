#pragma once

#include <memory>
#include "../demo_manager.hpp"
#include "../config.hpp"
#include "quantum/processor.h"
#include "memory/quantum_coherence_manager.h"

namespace sep {
namespace workbench {

class GenesisPatternDemo : public Demo {
public:
    void init() override;
    void update(float dt) override;
    void render() override;
    void cleanup() override;
    void handleKeyboard(unsigned char key) override;
    void handleMouse(int x, int y, int button) override;

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
        float coherence{0.0f};
        int   pattern_count{0};
        float evolution_rate{0.0f};
        int   iterations{0};
    } metrics_;

    std::unique_ptr<pattern::PatternProcessor> pattern_processor_;
    std::unique_ptr<QuantumCoherenceManager>   coherence_manager_;

    float evolution_rate_{0.1f};
    float coherence_threshold_{0.5f};
    bool  auto_evolve_{true};
};

} // namespace workbench
} // namespace sep
