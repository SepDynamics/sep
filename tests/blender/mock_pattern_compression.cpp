#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "memory/quantum_coherence_manager.hpp"

namespace sep::test {

class MockPatternCompression : public quantum::PatternCompression {
public:
    MOCK_METHOD(std::vector<uint8_t>, compressPattern, (const persistence::PatternData& quantum::Pattern), (override));
    MOCK_METHOD(persistence::PatternData, decompressPattern, (const std::vector<uint8_t>& compressed), (override));
    MOCK_METHOD(float, calculateCompressionRatio, (const persistence::PatternData& quantum::Pattern), (override));
    MOCK_METHOD(memory::CoherenceResult, validateCompression, (const persistence::PatternData& original, const persistence::PatternData& decompressed), (override));
};

} // namespace sep::test
