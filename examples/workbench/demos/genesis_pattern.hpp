#pragma once
#include "../demo_manager.hpp"
#include "config.hpp"
#include <memory>
#include <glm/glm.hpp>

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

    std::unique_ptr<sep::pattern::PatternProcessor> pattern_processor_;
    std::unique_ptr<sep::memory::QuantumCoherenceManager> coherence_manager_;

    struct ViewSettings {
        float rotation{0.f};
        float zoom{1.f};
        bool wireframe{false};
    } view_;

    float evolution_rate_{0.1f};
    float coherence_threshold_{0.5f};
    bool auto_evolve_{true};

    const float rate_step_{1.1f};
    const float max_rate_{5.0f};
    const float min_rate_{0.01f};
    const float rotation_sensitivity_{0.005f};
    const float zoom_sensitivity_{0.01f};
    const float min_zoom_{0.1f};
    const float max_zoom_{10.0f};
};

} // namespace workbench
} // namespace sep
