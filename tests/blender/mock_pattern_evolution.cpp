#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "quantum/pattern_evolution_bridge.h"
#include "memory/quantum_coherence_manager.h"
#include "pattern/pattern_data.h"

namespace sep::test {

class MockPatternEvolution : public quantum::PatternEvolutionBridge {
public:
    MOCK_METHOD(memory::CoherenceResult, evolvePattern, (const pattern::PatternData& pattern), (override));
    MOCK_METHOD(void, setEvolutionRate, (float rate), (override));
    MOCK_METHOD(void, setCouplingStrength, (float strength), (override));
    MOCK_METHOD(float, getCoherenceThreshold, (), (const, override));
    MOCK_METHOD(std::vector<pattern::PatternData>, getEvolvedPatterns, (), (const, override));
    MOCK_METHOD(bool, isEvolutionComplete, (), (const, override));
};

} // namespace sep::test