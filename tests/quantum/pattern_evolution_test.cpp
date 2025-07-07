#include <gtest/gtest.h>
#include "quantum/pattern_evolution.h"
#include "quantum/data.hpp"
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

namespace sep::quantum::mcp::test {

using sep::pattern::PatternData;
using sep::quantum::mcp::PatternEvolution;

class PatternEvolutionTest : public ::testing::Test {
 protected:
  void SetUp() override {}
};

TEST_F(PatternEvolutionTest, EvolvePatternBasic) {
  nlohmann::json data = {{"quantum_state", {0, 1, 2, 3}},
                         {"amplitudes", {0.5, 0.5, 0.5, 0.5}},
                         {"metadata", {{"test", "data"}}}};

  PatternData pattern = PatternEvolution::evolvePattern(data);

  EXPECT_TRUE(pattern.stability >= 0.0f && pattern.stability <= 1.0f);
  EXPECT_TRUE(pattern.coherence >= 0.0f && pattern.coherence <= 1.0f);
}

TEST_F(PatternEvolutionTest, GetPatternsEmpty) {
  auto patterns = PatternEvolution::getPatterns();
  EXPECT_TRUE(patterns.empty());
}

TEST_F(PatternEvolutionTest, ProcessPatternsBasic) {
  std::vector<PatternData> input;
  std::vector<PatternData> output;
  sep::pattern::PatternConfig config;
  config.update_threshold = 0.1f;
  config.enable_mutations = true;
  config.max_patterns = 10;
  config.batch_size = 2;

  auto result = PatternEvolution::processPatterns(input, config, output);
  EXPECT_EQ(result, pattern::PatternResult::SUCCESS);
}

TEST_F(PatternEvolutionTest, CalculateRelationshipStrength) {
  PatternData p1, p2;
  p1.coherence = 0.8f;
  p1.stability = 0.7f;
  p2.coherence = 0.9f;
  p2.stability = 0.8f;

  float strength = PatternEvolution::calculateRelationshipStrength(p1, p2);
  EXPECT_TRUE(strength >= 0.0f && strength <= 1.0f);
}

TEST_F(PatternEvolutionTest, JsonSerialization) {
  PatternData original;
  original.id = "test_pattern";
  original.generation = 5;
  original.coherence = 0.75f;
  original.stability = 0.8f;

  auto json = PatternEvolution::toJson(original);
  auto deserialized = PatternEvolution::fromJson(json);

  EXPECT_EQ(deserialized.id, original.id);
  EXPECT_EQ(deserialized.generation, original.generation);
  EXPECT_FLOAT_EQ(deserialized.coherence, original.coherence);
  EXPECT_FLOAT_EQ(deserialized.stability, original.stability);
}

TEST_F(PatternEvolutionTest, ProcessPatternsMemoryTiers) {
  std::vector<PatternData> input(3);
  std::vector<PatternData> output;
  sep::pattern::PatternConfig config;

  // STM pattern
  input[0].coherence = 0.1f;
  input[0].stability = 0.1f;
  input[0].memory_tier = sep::memory::MemoryTierEnum::STM;

  // MTM pattern
  input[1].coherence = 0.6f;
  input[1].stability = 0.4f;
  input[1].generation = 10;
  input[1].memory_tier = sep::memory::MemoryTierEnum::MTM;

  // LTM pattern
  input[2].coherence = 0.9f;
  input[2].stability = 0.8f;
  input[2].generation = 150;
  input[2].memory_tier = sep::memory::MemoryTierEnum::LTM;

  auto result = PatternEvolution::processPatterns(input, config, output);
  EXPECT_EQ(result, pattern::PatternResult::SUCCESS);
  EXPECT_EQ(output.size(), 3u);
}

TEST_F(PatternEvolutionTest, ProcessPatternsInvalidData) {
  std::vector<PatternData> input(1);
  std::vector<PatternData> output;
  sep::pattern::PatternConfig config;

  // Invalid coherence value
  input[0].coherence = 2.0f;
  input[0].stability = 0.5f;

  auto result = PatternEvolution::processPatterns(input, config, output);
  EXPECT_EQ(result, pattern::PatternResult::INVALID_ARGUMENT);
}

TEST_F(PatternEvolutionTest, ProcessPatternsMemoryPressure) {
  std::vector<PatternData> input;
  std::vector<PatternData> output;
  sep::pattern::PatternConfig config;
  config.max_patterns = 2;

  // Add more patterns than max_patterns
  input.resize(3);
  for (auto& p : input) {
    p.coherence = 0.5f;
    p.stability = 0.5f;
  }

  auto result = PatternEvolution::processPatterns(input, config, output);
  EXPECT_EQ(result, pattern::PatternResult::SUCCESS);
  EXPECT_LE(output.size(), 2u);
}

TEST_F(PatternEvolutionTest, QuantumStateTransitions) {
  nlohmann::json data = {
    {"quantum_state", {0, 1, 2, 3}},
    {"amplitudes", {0.5, 0.5, 0.5, 0.5}},
    {"phase", 0.0f}
  };

  PatternData pattern = PatternEvolution::evolvePattern(data);
  EXPECT_EQ(pattern.state, ::sep::quantum::QuantumState::Status::SUPERPOSITION);

  // Evolve pattern to trigger state transition
  pattern.coherence = 0.9f;
  pattern.stability = 0.8f;
  pattern.generation = 150;

  std::vector<PatternData> input{pattern};
  std::vector<PatternData> output;
  sep::pattern::PatternConfig config;

  auto result = PatternEvolution::processPatterns(input, config, output);
  EXPECT_EQ(result, pattern::PatternResult::SUCCESS);
  EXPECT_FALSE(output.empty());
  EXPECT_NE(output[0].state, ::sep::quantum::QuantumState::Status::SUPERPOSITION);
}

TEST_F(PatternEvolutionTest, EvolvePatternWithId) {
  nlohmann::json data = {{"quantum_state", {0, 1}}, {"amplitudes", {0.7071, 0.7071}}};

  std::string pattern_id = "test_pattern";
  PatternData pattern = PatternEvolution::evolvePattern(data, pattern_id);

  EXPECT_TRUE(pattern.coherence > 0.0f);
  EXPECT_TRUE(pattern.stability > 0.0f);
}

TEST_F(PatternEvolutionTest, EvolvePatternDefaultAmplitudes) {
  nlohmann::json data = {{"quantum_state", {0, 1, 2, 3}}};

  PatternData pattern = PatternEvolution::evolvePattern(data);

  EXPECT_TRUE(pattern.coherence > 0.0f);
}

TEST_F(PatternEvolutionTest, GetPatternsFiltering) {
  nlohmann::json args = {
      {"min_coherence", 0.7f},
      {"min_stability", 0.6f},
      {"patterns", nlohmann::json::array({{{"coherence", 0.8f}, {"stability", 0.7f}},
                                          {{"coherence", 0.6f}, {"stability", 0.5f}}})}};

  auto patterns = PatternEvolution::getPatterns(args);

  EXPECT_EQ(patterns.size(), 1);
  EXPECT_NEAR(patterns[0].coherence, 0.8f, 1e-6f);
}

TEST_F(PatternEvolutionTest, InvalidInput) {
  nlohmann::json invalid_data = {{"wrong_key", {0, 1}}};

  PatternData pattern = PatternEvolution::evolvePattern(invalid_data);
  EXPECT_GE(pattern.generation, 1u);
}

TEST_F(PatternEvolutionTest, MismatchedSizes) {
  nlohmann::json mismatched_data = {{"quantum_state", {0, 1}}, {"amplitudes", {0.5}}};

  PatternData pattern = PatternEvolution::evolvePattern(mismatched_data);
  EXPECT_GE(pattern.generation, 1u);
}

}  // namespace sep::quantum::mcp::test
