#include "neuro_sim.hpp"
#include <algorithm>

namespace sep {
namespace workbench {

void NeuroSimDemo::init() {
    mem_ = &memory::MemoryTierManager::getInstance();
    graph_ = &mem_->getDagGraph();

    const size_t neuron_count = 5;
    const float connection_prob = 0.3f;

    neurons_.resize(neuron_count);
    for(size_t i=0;i<neurons_.size();++i) {
        auto& n = neurons_[i];
        n.data.id = "neuron_" + std::to_string(i);
        n.data.coherence = 0.f;
        n.node_id = graph_->addNode(glm::vec3(0.0f), 0.0f, {});
        n.block = mem_->allocate(sizeof(Neuron), static_cast<memory::TierType>(memory::MemoryTierEnum::STM));
        mem_->registerPattern(n.node_id, n.data);
    }

    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    for(auto& n : neurons_) {
        std::vector<uint64_t> parents;
        for(const auto& other : neurons_) {
            if(n.node_id == other.node_id) continue;
            if(dist(rng_) < connection_prob) parents.push_back(other.node_id);
        }
        graph_->updateNodeParents(n.node_id, parents);
    }
}

void NeuroSimDemo::update(float dt) {
    std::vector<pattern::PatternData> patterns;
    patterns.reserve(neurons_.size());
    for(auto& n : neurons_) patterns.push_back(n.data);

    quantum::evolution::applySpikingLearning(patterns, *graph_, 1.0f, 0.1f);

    for(size_t i=0;i<neurons_.size();++i) {
        neurons_[i].data = patterns[i];
        graph_->updateCoherence(neurons_[i].node_id, patterns[i].coherence);
        mem_->updateBlockMetrics(neurons_[i].block, patterns[i].coherence, 1.0f, patterns[i].generation++, 1.0f);
    }
}

void NeuroSimDemo::render() {
    if(!renderer_) return;
    std::vector<glm::vec3> points;
    for(const auto& n : neurons_) {
        points.emplace_back(n.data.coherence);
    }
    renderer_->renderPatternState(points);
}

void NeuroSimDemo::cleanup() {
    for(auto& n : neurons_) {
        if(n.block) mem_->deallocate(n.block);
    }
    neurons_.clear();
}

void NeuroSimDemo::handleKeyboard(unsigned char) {}
void NeuroSimDemo::handleMouse(int, int, int) {}

} // namespace workbench
} // namespace sep
