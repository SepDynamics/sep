#include "quantum/evolution.h"

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <numeric>
#include <stdexcept>

#include "engine/internal/string_operators.h"

namespace sep::quantum {

namespace
{
inline float deterministicNoise(uint64_t& state)
{
    state = state * 1664525u + 1013904223u;
    return static_cast<float>(state & 0xFFFFFFFFu) / static_cast<float>(0xFFFFFFFFu);
}
} // namespace

    class EvolutionEngine::EvolutionEngineImpl
    {
    public:
        explicit EvolutionEngineImpl(sep::quantum::Processor* processor)
            : processor_(processor), generation_number_(0), noise_state_(0), current_stats_()
        {
            if (!processor)
            {
                throw std::invalid_argument("Processor cannot be null");
            }
        }

        BatchProcessingResult evolve(const EvolutionParams& params)
        {
            params_ = params;
            auto patterns = processor_->getPatterns();
            if (patterns.empty())
            {
                BatchProcessingResult result;
                result.success = false;
                result.error_message = "No patterns to evolve";
                return result;
            }

            std::vector<std::pair<std::string, float>> fitness_scores;
            for (const auto& pattern : patterns)
            {
                fitness_scores.push_back(std::make_pair(pattern.id, calculateFitness(pattern)));
            }
            std::sort(fitness_scores.begin(), fitness_scores.end(),
                      [](const auto& a, const auto& b) { return a.second > b.second; });

            std::vector<std::string> elite_ids;
            size_t elite_count = std::min(params.elite_count, patterns.size());
            for (size_t i = 0; i < elite_count; ++i)
            {
                elite_ids.push_back(fitness_scores[i].first);
            }

            std::vector<std::string> next_generation_ids = elite_ids;
            while (next_generation_ids.size() < patterns.size())
            {
                auto parent_ids = tournamentSelection(params.tournament_size, 2);
                if (parent_ids.size() >= 2)
                {
                    Pattern parent1 = processor_->getPattern(parent_ids[0]);
                    auto parent2 = processor_->getPattern(parent_ids[1]);
                    auto child = crossover(parent1, parent2);
                    if (nextFloat() < processor_->getConfig().mutation_rate)
                    {
                        child = mutate(child);
                    }
                    processor_->addPattern(child);
                    next_generation_ids.push_back(child.id);
                }
                else if (!parent_ids.empty())
                {
                    ProcessingResult result = processor_->mutatePattern(parent_ids[0]);
                    if (result.success)
                    {
                        next_generation_ids.push_back(result.pattern.id);
                    }
                }
            }

            for (const auto& pattern : patterns)
            {
                if (std::find(next_generation_ids.begin(), next_generation_ids.end(),
                              std::string(pattern.id.c_str())) == next_generation_ids.end())
                {
                    processor_->removePattern(pattern.id);
                }
            }

            auto proc_result = processor_->processAll();
            updateStats(patterns);
            generation_number_++;

            // Convert from sep::BatchProcessingResult to sep::quantum::BatchProcessingResult
            sep::quantum::BatchProcessingResult result;
            result.success = proc_result.success;
            result.error_message = proc_result.error_message;
            result.pattern = proc_result.pattern;
            result.results = proc_result.results;
            return result;
        }

        BatchProcessingResult evolveGeneration() { return evolve(params_); }

        Pattern crossover(const Pattern& parent1, const Pattern& parent2)
        {
            Pattern child;
            child.id = generatePatternId();
            child.parent_ids = {parent1.id, parent2.id};
            child.timestamp = getCurrentTimestamp();
            child.last_accessed = child.timestamp;
            child.last_modified = child.timestamp;

            float alpha = nextFloat();
            auto& state1 = parent1.quantum_state;
            auto& state2 = parent2.quantum_state;
            auto& child_state = child.quantum_state;

            child_state.coherence = glm::mix(state1.coherence, state2.coherence, alpha);
            child_state.phase = glm::mix(state1.phase, state2.phase, alpha);  // Add phase crossover
            child_state.stability = glm::mix(state1.stability, state2.stability, alpha);
            child_state.entropy = glm::mix(state1.entropy, state2.entropy, alpha);
            child_state.mutation_rate = glm::mix(state1.mutation_rate, state2.mutation_rate, alpha);

            child.position = parent1.position * (1.0f - alpha) + parent2.position * alpha;

            if (!parent1.data.empty() && !parent2.data.empty())
            {
                size_t size = std::min(parent1.data.get_size(), parent2.data.get_size());
                child.data.resize(size);
                for (size_t i = 0; i < size; ++i)
                {
                    child.data[i] = parent1.data[i] * (1.0f - alpha) + parent2.data[i] * alpha;
                }
            }

            return child;
        }

        Pattern mutate(const Pattern& pattern)
        {
            Pattern mutated = pattern;
            auto& state = mutated.quantum_state;
            float sigma = processor_->getConfig().mutation_rate;

            state.coherence =
                glm::clamp(state.coherence + (nextFloat() * 2.0f - 1.0f) * sigma, 0.0f, 1.0f);
            state.stability = glm::clamp(
                state.stability + (nextFloat() * 2.0f - 1.0f) * sigma * 0.5f, 0.0f, 1.0f);
            state.phase += (nextFloat() * 2.0f - 1.0f) * sigma * static_cast<float>(M_PI);  // Add phase mutation
            state.entropy =
                glm::clamp(state.entropy + (nextFloat() * 2.0f - 1.0f) * sigma * 2.0f, 0.0f, 1.0f);
            mutated.position +=
                glm::vec4((nextFloat() * 2.0f - 1.0f) * sigma, (nextFloat() * 2.0f - 1.0f) * sigma,
                          (nextFloat() * 2.0f - 1.0f) * sigma, 0.0f);

            state.mutation_count++;
            return mutated;
        }

        std::vector<std::string> selectElite(size_t count)
        {
            auto patterns = processor_->getPatterns();
            std::vector<std::pair<std::string, float>> fitness_scores;
            for (const auto& pattern : patterns)
            {
                fitness_scores.push_back(std::make_pair(pattern.id, calculateFitness(pattern)));
            }
            std::sort(fitness_scores.begin(), fitness_scores.end(),
                      [](const auto& a, const auto& b) { return a.second > b.second; });

            std::vector<std::string> elite_ids;
            count = std::min(count, fitness_scores.size());
            for (size_t i = 0; i < count; ++i)
            {
                elite_ids.push_back(fitness_scores[i].first);
            }
            return elite_ids;
        }

        std::vector<std::string> tournamentSelection(size_t tournament_size, size_t num_winners)
        {
            auto patterns = processor_->getPatterns();
            if (patterns.empty()) return {};

            std::vector<std::string> winners;
            for (size_t w = 0; w < num_winners; ++w)
            {
                std::vector<std::pair<std::string, float>> tournament;
                for (size_t i = 0; i < tournament_size; ++i)
                {
                    size_t idx = nextIndex(patterns.size());
                    tournament.push_back(std::make_pair(patterns[idx].id, calculateFitness(patterns[idx])));
                }
                auto winner = std::max_element(
                    tournament.begin(), tournament.end(),
                    [](const auto& a, const auto& b) { return a.second < b.second; });
                winners.push_back(winner->first);
            }
            return winners;
        }

        std::vector<std::string> rouletteWheelSelection(size_t count)
        {
            auto patterns = processor_->getPatterns();
            if (patterns.empty()) return {};

            std::vector<float> fitness_values;
            float total_fitness = 0.0f;
            for (const auto& pattern : patterns)
            {
                float fitness = calculateFitness(pattern);
                fitness_values.push_back(fitness);
                total_fitness += fitness;
            }

            std::vector<std::string> selected;

            for (size_t i = 0; i < count; ++i)
            {
                float value = nextFloat() * total_fitness;
                float cumulative = 0.0f;
                for (size_t j = 0; j < patterns.size(); ++j)
                {
                    cumulative += fitness_values[j];
                    if (cumulative >= value)
                    {
                        selected.push_back(patterns[j].id);
                        break;
                    }
                }
            }
            return selected;
        }

        float calculateFitness(const Pattern& pattern) const
        {
            const auto& state = pattern.quantum_state;
            float coherence_fitness = state.coherence * params_.coherence_weight;
            float stability_fitness = state.stability * params_.stability_weight;
            float entropy_penalty = (1.0f - state.entropy) * 0.2f;
            float diversity_bonus = calculatePatternDiversity(pattern) * params_.diversity_bonus;
            float pressure_factor = 1.0f / (1.0f + params_.pressure * state.generation * 0.001f);
            float fitness =
                (coherence_fitness + stability_fitness + entropy_penalty + diversity_bonus) *
                pressure_factor;
            return glm::clamp(fitness, 0.0f, 1.0f);
        }

        float calculateDiversity(const std::vector<Pattern>& patterns) const
        {
            if (patterns.size() < 2) return 1.0f;
            float total_distance = 0.0f;
            size_t comparisons = 0;
            for (size_t i = 0; i < patterns.size(); ++i)
            {
                for (size_t j = i + 1; j < patterns.size(); ++j)
                {
                    float distance = calculatePatternDistance(patterns[i], patterns[j]);
                    total_distance += distance;
                    comparisons++;
                }
            }
            return comparisons > 0 ? total_distance / comparisons : 0.0f;
        }

        void setParams(const EvolutionParams& params) { params_ = params; }
        EvolutionParams getParams() const { return params_; }
        EvolutionStats getStats() const { return current_stats_; }
        std::vector<EvolutionStats> getHistory() const { return stats_history_; }

    private:
        float calculatePatternDiversity(const Pattern& pattern) const
        {
            auto patterns = processor_->getPatterns();
            if (patterns.size() < 2) return 1.0f;
            float avg_distance = 0.0f;
            size_t count = 0;
            for (const auto& other : patterns)
            {
                if (other.id != pattern.id)
                {
                    avg_distance += calculatePatternDistance(pattern, other);
                    count++;
                }
            }
            return count > 0 ? avg_distance / count : 0.0f;
        }

        float calculatePatternDistance(const Pattern& p1, const Pattern& p2) const
        {
            float pos_distance = glm::length(p1.position - p2.position);
            float state_distance =
                std::abs(p1.quantum_state.coherence - p2.quantum_state.coherence) +
                std::abs(p1.quantum_state.stability - p2.quantum_state.stability) +
                std::abs(p1.quantum_state.entropy - p2.quantum_state.entropy);
            return (pos_distance + state_distance) * 0.5f;
        }

        void updateStats(const std::vector<Pattern>& patterns)
        {
            EvolutionStats stats;
            stats.generation_number = generation_number_;
            stats.population_size = patterns.size();
            if (!patterns.empty())
            {
                std::vector<float> fitness_values;
                for (const auto& pattern : patterns)
                {
                    fitness_values.push_back(calculateFitness(pattern));
                }
                stats.average_fitness =
                    std::accumulate(fitness_values.begin(), fitness_values.end(), 0.0f) /
                    patterns.size();
                stats.best_fitness =
                    *std::max_element(fitness_values.begin(), fitness_values.end());
                stats.worst_fitness =
                    *std::min_element(fitness_values.begin(), fitness_values.end());
                stats.diversity = calculateDiversity(patterns);
            }
            current_stats_ = stats;
            stats_history_.push_back(stats);
        }

        std::string generatePatternId() const
        {
            static std::atomic<uint64_t> counter{0};
            return "evo_pat_" + std::to_string(getCurrentTimestamp()) + "_" +
                   std::to_string(counter.fetch_add(1));
        }

        uint64_t getCurrentTimestamp() const
        {
            auto now = std::chrono::system_clock::now();
            return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch())
                .count();
        }

        float nextFloat() { return deterministicNoise(noise_state_); }

        size_t nextIndex(size_t max)
        {
            return static_cast<size_t>(nextFloat() * static_cast<float>(max));
        }

        sep::quantum::Processor* processor_;
        EvolutionParams params_;
        size_t generation_number_;
        uint64_t noise_state_;
        EvolutionStats current_stats_;
        std::vector<EvolutionStats> stats_history_;
    };

    EvolutionEngine::EvolutionEngine(sep::quantum::Processor* processor)
        : impl_(std::make_unique<EvolutionEngineImpl>(processor))
    {
    }

    sep::quantum::BatchProcessingResult EvolutionEngine::evolve(const EvolutionParams& params)
    {
        return impl_->evolve(params);
    }
    sep::quantum::BatchProcessingResult EvolutionEngine::evolveGeneration()
    {
        return impl_->evolveGeneration();
    }
    Pattern EvolutionEngine::crossover(const Pattern& parent1, const Pattern& parent2)
    {
        return impl_->crossover(parent1, parent2);
    }
    Pattern EvolutionEngine::mutate(const Pattern& pattern) { return impl_->mutate(pattern); }
    std::vector<std::string> EvolutionEngine::selectElite(size_t count)
    {
        return impl_->selectElite(count);
    }
    std::vector<std::string> EvolutionEngine::tournamentSelection(size_t tournament_size,
                                                                  size_t num_winners)
    {
        return impl_->tournamentSelection(tournament_size, num_winners);
    }
    std::vector<std::string> EvolutionEngine::rouletteWheelSelection(size_t count)
    {
        return impl_->rouletteWheelSelection(count);
    }
    float EvolutionEngine::calculateFitness(const Pattern& pattern) const
    {
        return impl_->calculateFitness(pattern);
    }
    float EvolutionEngine::calculateDiversity(const std::vector<Pattern>& patterns) const
    {
        return impl_->calculateDiversity(patterns);
    }
    void EvolutionEngine::setParams(const EvolutionParams& params) { impl_->setParams(params); }
    EvolutionParams EvolutionEngine::getParams() const { return impl_->getParams(); }
    EvolutionEngine::EvolutionStats EvolutionEngine::getStats() const { return impl_->getStats(); }
    std::vector<EvolutionEngine::EvolutionStats> EvolutionEngine::getHistory() const
    {
        return impl_->getHistory();
    }

    namespace evolution
    {

        Pattern gaussianMutation(const Pattern& pattern, float sigma)
        {
            Pattern mutated = pattern;
            auto& state = mutated.quantum_state;
            static uint64_t noise_state = 0;
            auto rnd = [&]() { return deterministicNoise(noise_state); };
            state.coherence =
                glm::clamp(state.coherence + (rnd() * 2.0f - 1.0f) * sigma, 0.0f, 1.0f);
            state.stability =
                glm::clamp(state.stability + (rnd() * 2.0f - 1.0f) * sigma * 0.5f, 0.0f, 1.0f);
            state.phase += (rnd() * 2.0f - 1.0f) * sigma * static_cast<float>(M_PI);  // Add phase mutation
            state.entropy =
                glm::clamp(state.entropy + (rnd() * 2.0f - 1.0f) * sigma * 2.0f, 0.0f, 1.0f);
            mutated.position +=
                glm::vec4((rnd() * 2.0f - 1.0f) * sigma, (rnd() * 2.0f - 1.0f) * sigma,
                          (rnd() * 2.0f - 1.0f) * sigma, 0.0f);
            state.mutation_count++;
            return mutated;
        }

        Pattern uniformMutation(const Pattern& pattern, float rate)
        {
            Pattern mutated = pattern;
            static uint64_t noise_state = 0;
            auto rnd = [&]() { return deterministicNoise(noise_state); };
            auto& state = mutated.quantum_state;
            if (rnd() < rate) state.coherence = rnd();
            if (rnd() < rate) state.stability = rnd();
            if (rnd() < rate) state.entropy = rnd();
            for (int i = 0; i < 3; ++i)
            {
                if (rnd() < rate) mutated.position[i] = rnd() * 2.0f - 1.0f;
            }
            state.mutation_count++;
            return mutated;
        }

        Pattern blendCrossover(const Pattern& parent1, const Pattern& parent2, float alpha)
        {
            Pattern child;
            child.parent_ids = {parent1.id, parent2.id};
            auto& state1 = parent1.quantum_state;
            auto& state2 = parent2.quantum_state;
            auto& child_state = child.quantum_state;
            child_state.coherence = glm::mix(state1.coherence, state2.coherence, alpha);
            child_state.phase = glm::mix(state1.phase, state2.phase, alpha);  // Add phase crossover
            child_state.stability = glm::mix(state1.stability, state2.stability, alpha);
            child_state.entropy = glm::mix(state1.entropy, state2.entropy, alpha);
            child.position = parent1.position * (1.0f - alpha) + parent2.position * alpha;
            return child;
        }

    }  // namespace evolution

}  // namespace sep::quantum

