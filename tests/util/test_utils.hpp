#ifndef SEP_TEST_UTILS_HPP
#define SEP_TEST_UTILS_HPP

#include <functional>
#include <gtest/gtest.h>
#include <string>
#include <vector>

namespace sep::test {

struct MutationResult {
  std::string description;
  std::string location;
  bool killed{false};
};

class TestWithMutation : public ::testing::Test {
protected:
  struct Mutation {
    std::string description;
    std::string location;
    std::function<bool()> test;
  };

  void SetUp() override { setupMutations(); }

  virtual void setupMutations() = 0;

  void registerMutation(const std::string &description,
                        const std::string &location,
                        std::function<bool()> test) {
    mutations_.push_back({description, location, test});
  }

  std::vector<MutationResult> runMutationAnalysis() {
    std::vector<MutationResult> results;
    for (const auto &mutation : mutations_) {
      MutationResult result;
      result.description = mutation.description;
      result.location = mutation.location;
      result.killed = mutation.test();
      results.push_back(result);
    }
    return results;
  }

private:
  std::vector<Mutation> mutations_;
};

} // namespace sep::test

#endif // SEP_TEST_UTILS_HPP
