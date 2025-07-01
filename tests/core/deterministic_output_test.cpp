#include <gtest/gtest.h>
#include "core/engine.h"
#include "compat/types.h"
#include "core/types.h"

using namespace sep;
using namespace sep::cuda;
using namespace sep::core;

namespace {
std::vector<PinState> makeStates(size_t count) {
    std::vector<PinState> states(count);
    for (size_t i = 0; i < count; ++i) {
        states[i].state = static_cast<uint32_t>(i);
        states[i].flags = ~0u;
        states[i].pin_id = static_cast<std::uint32_t>(i);
        states[i].value = static_cast<float>(i) + 1.0f;
        states[i].coherence = 0.75f;
    }
    return states;
}
}

TEST(EngineDeterminism, ConsistentGenerateProbes) {
    Engine engine;
    ASSERT_TRUE(engine.init(sep::config::APIConfig{}));
    auto inputs = makeStates(8);
    std::vector<uint32_t> idx1, exp1;
    engine.generate_probes(inputs, idx1, exp1, 0);

    std::vector<uint32_t> idx2, exp2;
    engine.generate_probes(inputs, idx2, exp2, 0);

    EXPECT_EQ(idx1, idx2);
    EXPECT_EQ(exp1, exp2);
}

TEST(EngineDeterminism, ConsistentProcessBatch) {
    Engine engine;
    ASSERT_TRUE(engine.init(sep::config::APIConfig{}));
    auto inputs = makeStates(8);

    QBSAResult qbsa1; QSHResult qsh1;
    engine.process_batch(inputs, 0, qbsa1, qsh1);

    QBSAResult qbsa2; QSHResult qsh2;
    engine.process_batch(inputs, 0, qbsa2, qsh2);

    EXPECT_EQ(qbsa1.correction_count, qbsa2.correction_count);
    EXPECT_EQ(qbsa1.correction_indices, qbsa2.correction_indices);
    EXPECT_EQ(qsh1.collapse_counts, qsh2.collapse_counts);
    EXPECT_EQ(qsh1.collapse_indices, qsh2.collapse_indices);
}

TEST(EngineDeterminism, DagLineageStable) {
    Engine engine;
    ASSERT_TRUE(engine.init(sep::config::APIConfig{}));
    auto inputs = makeStates(4);
    QBSAResult qbsa; QSHResult qsh;
    for (int t = 0; t < 3; ++t) {
        engine.process_batch(inputs, t, qbsa, qsh);
    }
    const auto& history = engine.getStateHistory();
    ASSERT_EQ(history.size(), 3u);
    EXPECT_EQ(history[1].parents.size(), 1u);
    EXPECT_EQ(history[1].parents[0], 0u);
    EXPECT_EQ(history[2].parents.size(), 1u);
    EXPECT_EQ(history[2].parents[0], 1u);
}

TEST(EngineDeterminism, MultipleRunsSameHistory) {
    auto runEngine = [](Engine& eng) {
        auto inputs = makeStates(6);
        QBSAResult qb; QSHResult qs;
        for (int t = 0; t < 2; ++t) {
            eng.process_batch(inputs, t, qb, qs);
        }
    };

    Engine e1; Engine e2;
    ASSERT_TRUE(e1.init(sep::config::APIConfig{}));
    ASSERT_TRUE(e2.init(sep::config::APIConfig{}));
    runEngine(e1);
    runEngine(e2);
    EXPECT_EQ(e1.getStateHistory().size(), e2.getStateHistory().size());
    for (size_t i = 0; i < e1.getStateHistory().size(); ++i) {
        EXPECT_EQ(e1.getStateHistory()[i].coherence, e2.getStateHistory()[i].coherence);
        EXPECT_EQ(e1.getStateHistory()[i].parents, e2.getStateHistory()[i].parents);
    }
}

TEST(EngineDeterminism, GenerateProbesDifferentTicks) {
    Engine engine;
    ASSERT_TRUE(engine.init(sep::config::APIConfig{}));
    auto inputs = makeStates(4);
    std::vector<uint32_t> a1,a2,b1,b2;
    engine.generate_probes(inputs,a1,b1,0);
    engine.generate_probes(inputs,a2,b2,1);
    EXPECT_EQ(a1,a2);
    EXPECT_EQ(b1,b2);
}

