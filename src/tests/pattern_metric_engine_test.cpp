#include <gtest/gtest.h>

#include <fstream>
#include <random>
#include <sstream>
#include <string>

#include "quantum/pattern_metric_engine.h"

using namespace sep::quantum;
using namespace sep::pattern;

class PatternMetricEngineTest : public ::testing::Test {
protected:
    void SetUp() override {
        engine_ = std::make_unique<PatternMetricEngine>();
        EXPECT_EQ(engine_->init(nullptr), sep::SEPResult::SUCCESS);
    }

    std::unique_ptr<PatternMetricEngine> engine_;
};

// Test binary data processing
TEST_F(PatternMetricEngineTest, ProcessBinaryData) {
    // Create binary test data
    std::vector<uint8_t> binary_data = {0x00, 0xFF, 0x80, 0x40, 0x20, 0x10};

    // Process binary data
    engine_->ingestData(binary_data.data(), binary_data.size());
    engine_->evolvePatterns();
    
    // Verify metrics
    auto metrics = engine_->computeMetrics();
    EXPECT_FALSE(metrics.empty());
    EXPECT_GE(metrics[0].coherence, 0.0f);
    EXPECT_LE(metrics[0].coherence, 1.0f);
    EXPECT_GE(metrics[0].stability, 0.0f);
    EXPECT_LE(metrics[0].stability, 1.0f);
}

// Test text string processing
TEST_F(PatternMetricEngineTest, ProcessTextData) {
    std::string text = "Hello, Quantum Pattern Processing!";

    // Process text data as bytes
    engine_->ingestData(reinterpret_cast<const uint8_t*>(text.data()), text.size());
    engine_->evolvePatterns();
    
    auto metrics = engine_->computeMetrics();
    EXPECT_FALSE(metrics.empty());
    EXPECT_GE(metrics[0].coherence, 0.0f);
    EXPECT_LE(metrics[0].coherence, 1.0f);
}

// Test numeric array processing
TEST_F(PatternMetricEngineTest, ProcessNumericArray) {
    std::vector<float> numbers = {1.0f, 2.5f, 3.7f, 4.2f, 5.0f};

    // Convert floats to bytes for processing
    std::vector<uint8_t> bytes(numbers.size() * sizeof(float));
    std::memcpy(bytes.data(), numbers.data(), bytes.size());
    
    engine_->ingestData(bytes.data(), bytes.size());
    engine_->evolvePatterns();
    
    auto metrics = engine_->computeMetrics();
    EXPECT_FALSE(metrics.empty());
    EXPECT_GE(metrics[0].stability, 0.0f);
    EXPECT_LE(metrics[0].stability, 1.0f);
}

// Test random byte stream processing
TEST_F(PatternMetricEngineTest, ProcessRandomStream) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);
    
    // Create random byte stream
    std::stringstream stream;
    for (int i = 0; i < 1000; ++i) {
        stream.put(static_cast<char>(dis(gen)));
    }
    
    // Process stream
    engine_->ingestData(stream);
    engine_->evolvePatterns();
    
    auto metrics = engine_->computeMetrics();
    EXPECT_FALSE(metrics.empty());
    EXPECT_GE(metrics[0].entropy, 0.0f);
}

// Test edge cases
TEST_F(PatternMetricEngineTest, HandleEdgeCases) {
    // Empty data
    engine_->ingestData(nullptr, 0);
    auto metrics = engine_->computeMetrics();
    EXPECT_TRUE(metrics.empty());
    
    // Single byte
    uint8_t single_byte = 0x42;
    engine_->ingestData(&single_byte, 1);
    engine_->evolvePatterns();
    metrics = engine_->computeMetrics();
    EXPECT_FALSE(metrics.empty());
    
    // Large data (>64 byte chunks)
    std::vector<uint8_t> large_data(1000, 0x55);
    engine_->ingestData(large_data.data(), large_data.size());
    engine_->evolvePatterns();
    metrics = engine_->computeMetrics();
    EXPECT_FALSE(metrics.empty());
}

// Test pattern mutation
TEST_F(PatternMetricEngineTest, PatternMutation) {
    sep::pattern::PatternData p;
    p.id = "pattern_0";
    p.data.push_back(1.0f);
    engine_->addPattern(p);
    
    auto patterns = engine_->getPatterns();
    EXPECT_FALSE(patterns.empty());
    
    auto mutated = engine_->mutatePattern(patterns[0]);
    EXPECT_NE(mutated.id, patterns[0].id);
    EXPECT_GT(mutated.generation, patterns[0].generation);
    EXPECT_FALSE(mutated.data.empty());
}

// Test repetitive data
TEST_F(PatternMetricEngineTest, ProcessRepetitiveData) {
    engine_->clear();
    std::ifstream f("assets/test_data/repetitive_data.bin", std::ios::binary);
    ASSERT_TRUE(f.is_open()) << "Failed to open repetitive test data file.";

    std::vector<uint8_t> bytes((std::istreambuf_iterator<char>(f)), {});
    ASSERT_FALSE(bytes.empty()) << "Repetitive test data file is empty.";
    
    engine_->ingestData(bytes.data(), bytes.size());
    engine_->evolvePatterns();
    auto metrics = engine_->computeMetrics();
    
    ASSERT_FALSE(metrics.empty());
    // Expect high coherence for repetitive data
    for (const auto& m : metrics) {
        EXPECT_GT(m.coherence, 0.8f);
    }
}

// Test random data
TEST_F(PatternMetricEngineTest, ProcessRandomData) {
    engine_->clear();
    std::ifstream f("assets/test_data/random_data.bin", std::ios::binary);
    ASSERT_TRUE(f.is_open()) << "Failed to open random test data file.";

    std::vector<uint8_t> bytes((std::istreambuf_iterator<char>(f)), {});
    ASSERT_FALSE(bytes.empty()) << "Random test data file is empty.";
    
    engine_->ingestData(bytes.data(), bytes.size());
    engine_->evolvePatterns();
    
    auto metrics = engine_->computeMetrics();
    
    ASSERT_FALSE(metrics.empty());
    
    // Expect non-uniform coherence for random data
    bool all_high = true;
    for (const auto& m : metrics) {
        if (m.coherence < 0.8f) {
            all_high = false;
            break;
        }
    }
    EXPECT_FALSE(all_high) << "Coherence for random data should not be uniformly high";
}