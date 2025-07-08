#include "utils/dataset_loader.h"
#include <gtest/gtest.h>
#include <cmath>

using namespace sep::utils;

static void compareVec(const glm::vec3 &a, const glm::vec3 &b) {
    EXPECT_NEAR(a.x, b.x, 1e-6);
    EXPECT_NEAR(a.y, b.y, 1e-6);
    EXPECT_NEAR(a.z, b.z, 1e-6);
}

TEST(DatasetLoader, JsonDataset) {
    auto data = loadDataset(std::string(SEP_TEST_DATA_DIR) + "/sample.json");
    ASSERT_EQ(data.size(), 2);
    compareVec(data[0], glm::vec3(0.1f,0.2f,0.3f));
    compareVec(data[1], glm::vec3(0.4f,0.5f,0.6f));
}

TEST(DatasetLoader, BinaryDataset) {
    auto data = loadDataset(std::string(SEP_TEST_DATA_DIR) + "/sample.bin");
    ASSERT_EQ(data.size(), 2);
    compareVec(data[0], glm::vec3(0.1f,0.2f,0.3f));
    compareVec(data[1], glm::vec3(0.4f,0.5f,0.6f));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
