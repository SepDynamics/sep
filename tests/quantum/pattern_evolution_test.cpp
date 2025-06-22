#include <gtest/gtest.h>


#include <nlohmann/json.hpp>
#include <string>
#include <vector>

namespace sep::quantum::mcp::test {

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
