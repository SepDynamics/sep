#include "drug_optimizer.hpp"

#include <cstdlib>
// Include glm_config.h before any GLM headers to ensure GLM_ENABLE_EXPERIMENTAL is defined
#include <glm/gtx/norm.hpp>

#include "engine/glm_config.h"

// Use namespace alias to avoid ambiguity
namespace sq = sep::quantum;
using sep::compat::PatternData;

namespace sep {
namespace workbench {

    void DrugOptimizerDemo::on_load(sep::core::Engine* engine, sep::CyclesRenderer* renderer)
    {
        (void)engine;
        renderer_ = renderer;
        poses_.resize(5);
        for (auto& p : poses_)
        {
            p.position = glm::vec3(static_cast<float>(std::rand()) / RAND_MAX,
                                   static_cast<float>(std::rand()) / RAND_MAX,
                                   static_cast<float>(std::rand()) / RAND_MAX);
            p.orientation = glm::vec3(0.0f);
            p.binding_affinity = computeBindingScore(p);
        }
    }

float DrugOptimizerDemo::computeBindingScore(const MoleculePose& pose) {
#ifdef SEP_EXT_CHEM
    // Placeholder for external chemistry library integration
    return external_chemistry_score(pose.position.x, pose.position.y, pose.position.z);
#else
    return 1.0f / (1.0f + glm::length2(pose.position));
#endif
}

void DrugOptimizerDemo::on_update(float) {
    // Implement our own optimization logic instead of using the optimizer
    // This avoids the type compatibility issues
    
    // Create a copy of the poses for optimization
    std::vector<MoleculePose> optimized_poses = poses_;
    
    // Simple optimization: move each pose slightly towards a better binding score
    for (auto& pose : optimized_poses) {
        // Try small random movements and keep the best one
        MoleculePose best_pose = pose;
        float best_score = pose.binding_affinity;
        
        for (int i = 0; i < 10; ++i) {
            MoleculePose test_pose = pose;
            // Apply small random perturbation
            test_pose.position += glm::vec3(
                (static_cast<float>(std::rand()) / RAND_MAX - 0.5f) * 0.1f,
                (static_cast<float>(std::rand()) / RAND_MAX - 0.5f) * 0.1f,
                (static_cast<float>(std::rand()) / RAND_MAX - 0.5f) * 0.1f
            );
            
            // Compute new binding score
            float score = computeBindingScore(test_pose);
            
            // Keep if better
            if (score > best_score) {
                best_pose = test_pose;
                best_score = score;
            }
        }
        
        // Update with the best found position
        pose = best_pose;
    }
    
    // Update the original poses with the optimized ones
    poses_ = optimized_poses;
}

void DrugOptimizerDemo::on_render() {
    if (!renderer_) return;
    std::vector<glm::vec3> points;
    for (const auto& p : poses_) points.push_back(p.position);
    renderer_->renderPatternState(points);
}

void DrugOptimizerDemo::on_unload() { poses_.clear(); }

void DrugOptimizerDemo::on_ui_render()
{
    ImGui::Begin("Neural Simulation Controls");
    ImGui::SliderFloat("Threshold", &threshold_, 0.1f, 2.0f);
    ImGui::SliderFloat("Decay Rate", &decay_, 0.01f, 0.5f);
    ImGui::SliderFloat("Input Strength", &input_strength_, 0.1f, 1.0f);
    ImGui::SliderFloat("Learning Rate", &learning_rate_, 0.01f, 0.2f);
    ImGui::SliderFloat("Connection Probability", &connection_prob_, 0.1f, 0.5f);
    ImGui::End();
}

void DrugOptimizerDemo::on_key_press(int key) { (void)key; }
void DrugOptimizerDemo::on_mouse(int, int, int) {}

} // namespace workbench
} // namespace sep
