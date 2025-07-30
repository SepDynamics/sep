#ifndef SEP_QUANTUM_EVOLUTION_H
#define SEP_QUANTUM_EVOLUTION_H

#include <memory>
#include <vector>

#include "engine/internal/types.h"
#include "quantum/processor.h"

namespace sep::quantum {

struct BatchProcessingResult : public ProcessingResult {
    std::string message{};
    std::vector<ProcessingResult> results{};
};


struct EvolutionParams {
    float mutation_rate{0.1f};
    float crossover_rate{0.7f};
    uint32_t generations{100};
    size_t elite_count{5};
    size_t tournament_size{3};
    float coherence_weight{1.0f};
    float stability_weight{1.0f};
    float diversity_bonus{0.1f};
    float pressure{1.0f};
};

class EvolutionEngine {
public:
    struct EvolutionStats {
        size_t generation_number{0};
        size_t population_size{0};
        float average_fitness{0.0f};
        float best_fitness{0.0f};
        float worst_fitness{0.0f};
        float diversity{0.0f};
    };


    explicit EvolutionEngine(sep::quantum::Processor* processor);

    BatchProcessingResult evolve(const EvolutionParams& params);
    BatchProcessingResult evolveGeneration();

    Pattern crossover(const Pattern& parent1, const Pattern& parent2);
    Pattern mutate(const Pattern& pattern);

    std::vector<std::string> selectElite(size_t count);
    std::vector<std::string> tournamentSelection(size_t tournament_size, size_t num_winners);
    std::vector<std::string> rouletteWheelSelection(size_t count);

    float calculateFitness(const Pattern& pattern) const;
    float calculateDiversity(const std::vector<Pattern>& patterns) const;

    void setParams(const EvolutionParams& params);
    EvolutionParams getParams() const;
    EvolutionStats getStats() const;
    std::vector<EvolutionStats> getHistory() const;

private:
    class EvolutionEngineImpl;
    std::unique_ptr<EvolutionEngineImpl> impl_;
};

namespace evolution {
Pattern gaussianMutation(const Pattern& pattern, float sigma);
Pattern uniformMutation(const Pattern& pattern, float rate);
Pattern blendCrossover(const Pattern& parent1, const Pattern& parent2, float alpha);
float coherenceFitness(const Pattern& pattern);
float stabilityFitness(const Pattern& pattern);
float complexityFitness(const Pattern& pattern);
std::vector<Pattern> createRandomPopulation(size_t size);
void applySpike(Pattern& neuron, float input, float decay, float threshold);
void hebbianUpdate(const Pattern& pre, Pattern& post, float rate);
void applyGravity(Pattern& pattern, const glm::vec3& center, float strength);
void randomPerturbation(Pattern& pattern, float amplitude);
} // namespace evolution

BatchProcessingResult evolve(const EvolutionParams& params);
BatchProcessingResult evolveGeneration();

} // namespace sep::quantum

#endif // SEP_QUANTUM_EVOLUTION_H
