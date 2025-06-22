#include "context/resource_predictor.h"
#include "context/types.h"
#include <gtest/gtest.h>
#include <memory>

using namespace sep::context;

namespace sep::context {
std::unique_ptr<ResourcePredictor> createResourcePredictor(size_t history_size);
}

TEST(ResourcePredictorImplUsage, PredictFromHistory) {
    auto predictor = createResourcePredictor(50);
    UsagePattern pattern;
    pattern.batch_size = 10;
    pattern.processing_time = 1.0;
    pattern.memory_used = 1000;
    pattern.cpu_utilization = 0.2f;
    pattern.gpu_utilization = 0.5f;
    for (int i = 0; i < 10; ++i) {
        predictor->recordUsagePattern(pattern);
    }

    Batch batch;
    batch.contexts.resize(20);
    auto pred = predictor->predictResourceNeeds(batch);

    EXPECT_NEAR(pred.estimated_memory, 2200u, 250u);
    EXPECT_NEAR(pred.expected_processing_time, 2.0, 0.5);
    EXPECT_EQ(pred.optimal_batch_size, 10u);
}

TEST(ResourcePredictorImplUsage, RejectsAnomalies) {
    auto predictor = createResourcePredictor(50);
    UsagePattern normal;
    normal.batch_size = 10;
    normal.processing_time = 1.0;
    normal.memory_used = 1000;
    predictor->recordUsagePattern(normal);

    UsagePattern anomaly = normal;
    anomaly.memory_used = 20000;
    anomaly.processing_time = 20.0;
    predictor->recordUsagePattern(anomaly);

    auto metrics = predictor->getResourceMetrics();
    EXPECT_LT(metrics.average_memory_usage, 5000u);
}

TEST(ResourcePredictorImplUsage, BatchSuggestionUsesHistory) {
    auto predictor = createResourcePredictor(50);
    UsagePattern pattern{10,1.0,1000,0.1f,0.2f};
    for(int i=0;i<5;++i) predictor->recordUsagePattern(pattern);
    predictor->updateResourceLimits(3000,1.0f,1.0f);
    auto batches = predictor->suggestBatchSizes(25);
    ASSERT_EQ(batches.size(),3u);
    EXPECT_EQ(batches[0],10u);
    EXPECT_EQ(batches[1],10u);
    EXPECT_EQ(batches[2],5u);
}

