#include "core/system_hooks.h"

#include "quantum/types.h"
#include <gtest/gtest.h>

using namespace sep;

class RecordingHooks : public core::SystemHooks {
public:
    void onPatternProcessed(std::size_t id) override { processed.push_back(id); }
    void onPatternPromoted(std::size_t id) override { promoted.push_back(id); }
    void onPatternDemoted(std::size_t id) override { demoted.push_back(id); }

    std::vector<std::size_t> processed;
    std::vector<std::size_t> promoted;
    std::vector<std::size_t> demoted;
};

TEST(SystemHooksTest, ReceivesEvents) {
    RecordingHooks hooks;
    pattern::PatternProcessor processor;
    processor.setHooks(&hooks);

    blender::PatternData pat;
    pat.id = "p1";
    pat.coherence = 0.8f;
    pat.stability = 0.9f;
    pat.entropy = 0.2f;
    pat.generation = 10;
    pat.memory_tier = MemoryTier::STM;

    processor.addPattern(pat);
    processor.evolvePatterns();
    processor.promotePatterns();

    EXPECT_GE(hooks.processed.size(), 1u);
    EXPECT_FALSE(hooks.promoted.empty() && hooks.demoted.empty());
}

