#include "embeddings/simple_embedding_model.h"
#include <gtest/gtest.h>

TEST(SimpleEmbeddingModelTestbed, Deterministic) {
    sep::embeddings::SimpleEmbeddingModel model;
    auto e1 = model.compute("abc");
    auto e2 = model.compute("abc");
    EXPECT_EQ(e1, e2);
}

