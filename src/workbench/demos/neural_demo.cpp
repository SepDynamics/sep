#include "neural_demo.hpp"

#include <algorithm>

#include "../simple_renderer.h"
#include "config.hpp"

namespace sep
{
    namespace workbench
    {

        void NeuralDemo::on_load(sep::core::Engine* engine, sep::SimpleRenderer* renderer)
        {
            (void)engine;
            renderer_ = renderer;
            const auto& cfg = Config::getInstance().neural_demo();
            int count = cfg.network.neuron_count;
            threshold_ = cfg.neuron.threshold;
            decay_ = cfg.neuron.decay;
            weight_ = cfg.network.connection_prob;

            neurons_.clear();
            graph_ = dag::DagGraph();
            for (int i = 0; i < count; ++i)
            {
                uint64_t id = graph_.addNode(glm::vec3(0.0f), 0.f, {});
                neurons_.push_back({id, 0.f});
                if (i > 0)
                {
                    graph_.updateNodeParents(neurons_[i].id, {neurons_[i - 1].id});
                }
            }
        }

        void NeuralDemo::on_update(float dt)
        {
            std::vector<float> inputs(neurons_.size(), 0.f);
            for (size_t i = 0; i < neurons_.size(); ++i)
            {
                auto parents = graph_.getParents(neurons_[i].id);
                for (uint64_t pid : parents)
                {
                    auto it = std::find_if(neurons_.begin(), neurons_.end(),
                                           [pid](const Neuron& n) { return n.id == pid; });
                    if (it != neurons_.end())
                    {
                        inputs[i] += it->potential * weight_;
                    }
                }
            }

            for (size_t i = 0; i < neurons_.size(); ++i)
            {
                Neuron& n = neurons_[i];
                n.potential += inputs[i];
                n.potential -= decay_ * n.potential * dt;
                if (n.potential >= threshold_)
                {
                    n.potential = 0.f;
                    for (size_t j = 0; j < neurons_.size(); ++j)
                    {
                        auto parents = graph_.getParents(neurons_[j].id);
                        if (std::find(parents.begin(), parents.end(), n.id) != parents.end())
                        {
                            neurons_[j].potential += 1.f;
                        }
                    }
                }
                graph_.updateCoherence(n.id, n.potential / threshold_);
            }
        }

        void NeuralDemo::on_render()
        {
            if (!renderer_) return;
            std::vector<glm::vec3> points;
            for (size_t i = 0; i < neurons_.size(); ++i)
            {
                float y = neurons_[i].potential / threshold_;
                points.push_back(glm::vec3(static_cast<float>(i), y, 0.f));
            }
            renderer_->renderPatternState(points);
        }

        void NeuralDemo::on_unload()
        {
            neurons_.clear();
            graph_ = dag::DagGraph();
        }

        void NeuralDemo::on_ui_render()
        {
            ImGui::Begin("Neural Simulation Controls");
            ImGui::SliderFloat("Threshold", &threshold_, 0.1f, 2.0f);
            ImGui::SliderFloat("Decay Rate", &decay_, 0.01f, 0.5f);
            ImGui::SliderFloat("Input Strength", &input_strength_, 0.1f, 1.0f);
            ImGui::SliderFloat("Learning Rate", &learning_rate_, 0.01f, 0.2f);
            ImGui::SliderFloat("Connection Probability", &connection_prob_, 0.1f, 0.5f);
            ImGui::End();
        }

        void NeuralDemo::on_key_press(int key) { (void)key; }
        void NeuralDemo::on_mouse(int, int, int) {}

    }  // namespace workbench
}  // namespace sep
