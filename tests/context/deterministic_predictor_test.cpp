#include <gtest/gtest.h>
#include "mock_resource_predictor.hpp"
#include "context/types.h"

using namespace sep::context;

TEST(ResourcePredictorDeterminism, PredictResourceNeedsStable) {
    MockResourcePredictor predictor(50);
    Batch batch; batch.contexts.resize(10);
    auto r1 = predictor.predictResourceNeeds(batch);
    auto r2 = predictor.predictResourceNeeds(batch);
    EXPECT_EQ(r1.estimated_memory, r2.estimated_memory);
    EXPECT_EQ(r1.optimal_batch_size, r2.optimal_batch_size);
    EXPECT_DOUBLE_EQ(r1.expected_processing_time, r2.expected_processing_time);
    EXPECT_FLOAT_EQ(r1.confidence_score, r2.confidence_score);
}

