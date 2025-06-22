#include "blender/api.h"


#include <gtest/gtest.h>

#include <cstring>
#include <memory>
#include <vector>

#include "blender/mock_types.h"

class APITest : public ::testing::Test {
 protected:
  void SetUp() override {
    // GPU context is not used by the API (cast to void), so we can use nullptr
    gpu_ctx_ = nullptr;

    // Set up test mesh
    mesh_ = std::make_unique<Mesh>();
    mesh_->totvert = 1000;
    mesh_->totedge = 2000;
    mesh_->totloop = 0;
    mesh_->totpoly = 0;
    mesh_->mvert = new MVert[mesh_->totvert];
    mesh_->medge = new MEdge[mesh_->totedge];
    mesh_->mloop = nullptr;
    mesh_->mpoly = nullptr;

    // Set up test object
    object_ = std::make_unique<Object>();
    object_->type = OB_MESH;
    object_->data = mesh_.get();
    std::memset(object_->name, 0, sizeof(object_->name));
    strcpy(object_->name, "TestObject");
  }

  void TearDown() override {
    if (bridge_) {
      sep_blender_cleanup(bridge_);
      bridge_ = nullptr;
    }

    delete[] mesh_->mvert;
    delete[] mesh_->medge;
  }

  GPUContext* gpu_ctx_;
  std::unique_ptr<Object> object_;
  std::unique_ptr<Mesh> mesh_;
  SEPBlenderBridge* bridge_ = nullptr;
};

TEST_F(APITest, InitializationTest) {
  SEPBlenderBridge* bridge = nullptr;
  EXPECT_EQ(SEPResult::SUCCESS, sep_blender_init(gpu_ctx_, nullptr, &bridge));
  EXPECT_NE(nullptr, bridge);
  sep_blender_cleanup(bridge);
}

TEST_F(APITest, NullContextInitTest) {
  SEPBlenderBridge* bridge = nullptr;
  EXPECT_EQ(SEPResult::INIT_FAILED, sep_blender_init(nullptr, nullptr, &bridge));
  EXPECT_EQ(nullptr, bridge);
}

TEST_F(APITest, RegisterMeshTest) {
  ASSERT_EQ(SEPResult::SUCCESS, sep_blender_init(gpu_ctx_, nullptr, &bridge_));

  SEPMeshHandle handle;
  EXPECT_EQ(SEPResult::SUCCESS, sep_register_mesh(bridge_, object_.get(), mesh_.get(), &handle));
  EXPECT_NE(0u, handle);
}

TEST_F(APITest, InvalidMeshTest) {
  ASSERT_EQ(SEPResult::SUCCESS, sep_blender_init(gpu_ctx_, nullptr, &bridge_));

  SEPMeshHandle handle;
  object_->type = 0;  // Not a mesh
  EXPECT_EQ(SEPResult::INVALID_OBJECT,
            sep_register_mesh(bridge_, object_.get(), mesh_.get(), &handle));
}

TEST_F(APITest, UpdateMeshTest) {
  ASSERT_EQ(SEPResult::SUCCESS, sep_blender_init(gpu_ctx_, nullptr, &bridge_));

  SEPMeshHandle handle;
  ASSERT_EQ(SEPResult::SUCCESS, sep_register_mesh(bridge_, object_.get(), mesh_.get(), &handle));

  SEPPatternMetrics metrics{};
  metrics.active_patterns = 100;
  metrics.avg_coherence = 0.8f;
  metrics.peak_entropy = 0.5f;

  bool updated = false;
  EXPECT_EQ(SEPResult::SUCCESS, sep_update_mesh(bridge_, handle, &metrics, &updated));
  EXPECT_TRUE(updated);
}

TEST_F(APITest, ProcessAudioTest) {
  ASSERT_EQ(SEPResult::SUCCESS, sep_blender_init(gpu_ctx_, nullptr, &bridge_));

  std::vector<float> samples(1024, 0.0f);
  SEPAudioMetrics metrics{};
  EXPECT_EQ(SEPResult::SUCCESS,
            sep_process_audio(bridge_, samples.data(), samples.size(), &metrics));
  EXPECT_EQ(metrics.frames_processed, samples.size());
  EXPECT_FLOAT_EQ(metrics.peak_level, 0.0f);
  EXPECT_FLOAT_EQ(metrics.rms_level, 0.0f);
}

TEST_F(APITest, SyncMemoryTest) {
  ASSERT_EQ(SEPResult::SUCCESS, sep_blender_init(gpu_ctx_, nullptr, &bridge_));
  EXPECT_EQ(SEPResult::SUCCESS,
            sep_sync_memory(bridge_, sep::MemoryTierEnum::STM, true));
}

TEST_F(APITest, GetMetricsTest) {
  ASSERT_EQ(SEPResult::SUCCESS, sep_blender_init(gpu_ctx_, nullptr, &bridge_));

  SEPPatternMetrics metrics{};
  EXPECT_EQ(SEPResult::SUCCESS, sep_get_metrics(bridge_, &metrics));
}

TEST_F(APITest, ResetStateTest) {
  ASSERT_EQ(SEPResult::SUCCESS, sep_blender_init(gpu_ctx_, nullptr, &bridge_));

  std::vector<float> samples(512, 0.5f);
  SEPAudioMetrics metrics{};
  ASSERT_EQ(SEPResult::SUCCESS,
            sep_process_audio(bridge_, samples.data(), samples.size(), &metrics));
  EXPECT_GT(metrics.peak_level, 0.0f);

  EXPECT_EQ(SEPResult::SUCCESS, sep_reset_state(bridge_));

  SEPAudioMetrics metrics_after{};
  ASSERT_EQ(SEPResult::SUCCESS,
            sep_process_audio(bridge_, samples.data(), samples.size(), &metrics_after));
  EXPECT_EQ(metrics_after.frames_processed, samples.size());
  EXPECT_NEAR(metrics_after.peak_level, metrics.peak_level, 1e-5);
}

TEST_F(APITest, VersionInfoTest) {
  EXPECT_NE(nullptr, sep_get_version());
  EXPECT_NE(nullptr, sep_get_build_info());
  EXPECT_STRNE("", sep_get_version());
  EXPECT_STRNE("", sep_get_build_info());
}

