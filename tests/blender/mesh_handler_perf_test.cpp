#include <gtest/gtest.h>

#include <chrono>
#include <memory>
#include <vector>

#include "blender/compat/blender_types.h"
#include "blender/mesh_handler.h"
#include "quantum/types.h"

using namespace sep::pattern;

class MeshHandlerPerfTest : public ::testing::Test {
 protected:
  void SetUp() override {
    setupLargeMesh();
    setupPatterns();
  }

  void TearDown() override {
    delete[] mesh_->mvert;
    delete[] mesh_->medge;
    delete[] mesh_->mloop;
    delete[] mesh_->mpoly;
  }

  void setupLargeMesh() {
    // Create large test mesh (100x100 grid = 10000 vertices)
    const int GRID_SIZE = 100;
    mesh_ = std::make_unique<Mesh>();
    mesh_->totvert = GRID_SIZE * GRID_SIZE;
    mesh_->totedge = (GRID_SIZE - 1) * GRID_SIZE * 2;
    mesh_->totpoly = (GRID_SIZE - 1) * (GRID_SIZE - 1);

    // Allocate arrays
    mesh_->mvert = new MVert[mesh_->totvert];
    mesh_->medge = new MEdge[mesh_->totedge];
    mesh_->mloop = new MLoop[mesh_->totpoly * 4];
    mesh_->mpoly = new MPoly[mesh_->totpoly];

    // Initialize grid vertices
    for (int i = 0; i < GRID_SIZE; ++i) {
      for (int j = 0; j < GRID_SIZE; ++j) {
        int idx = i * GRID_SIZE + j;
        mesh_->mvert[idx].co[0] = static_cast<float>(i);
        mesh_->mvert[idx].co[1] = static_cast<float>(j);
        mesh_->mvert[idx].co[2] = 0.0f;
      }
    }

    // Initialize grid edges
    int edge_idx = 0;
    // Horizontal edges
    for (int i = 0; i < GRID_SIZE; ++i) {
      for (int j = 0; j < GRID_SIZE - 1; ++j) {
        mesh_->medge[edge_idx].v1 = i * GRID_SIZE + j;
        mesh_->medge[edge_idx].v2 = i * GRID_SIZE + j + 1;
        edge_idx++;
      }
    }
    // Vertical edges
    for (int i = 0; i < GRID_SIZE - 1; ++i) {
      for (int j = 0; j < GRID_SIZE; ++j) {
        mesh_->medge[edge_idx].v1 = i * GRID_SIZE + j;
        mesh_->medge[edge_idx].v2 = (i + 1) * GRID_SIZE + j;
        edge_idx++;
      }
    }

    // Initialize grid faces
    int face_idx = 0;
    int loop_idx = 0;
    for (int i = 0; i < GRID_SIZE - 1; ++i) {
      for (int j = 0; j < GRID_SIZE - 1; ++j) {
        mesh_->mpoly[face_idx].loopstart = loop_idx;
        mesh_->mpoly[face_idx].totloop = 4;

        mesh_->mloop[loop_idx].v = i * GRID_SIZE + j;
        mesh_->mloop[loop_idx + 1].v = i * GRID_SIZE + j + 1;
        mesh_->mloop[loop_idx + 2].v = (i + 1) * GRID_SIZE + j + 1;
        mesh_->mloop[loop_idx + 3].v = (i + 1) * GRID_SIZE + j;

        face_idx++;
        loop_idx += 4;
      }
    }

    // Create test object
    object_ = std::make_unique<Object>();
    object_->type = OB_MESH;
    object_->data = mesh_.get();

    // Initialize handler
    handler_ = std::make_unique<MeshHandler>();
    ASSERT_EQ(sep::SEPResult::SUCCESS, handler_->init(object_.get(), mesh_.get()));
  }

  void setupPatterns() {
    patterns_.resize(100);
    for (size_t i = 0; i < patterns_.size(); ++i) {
      float v = static_cast<float>(i) / static_cast<float>(patterns_.size());
      patterns_[i].coherence = v;
      patterns_[i].entropy = v * 0.5f;
      patterns_[i].stability = 1.0f - v;
      patterns_[i].mutation_rate = v * 0.1f;
      patterns_[i].generation = 1;
      patterns_[i].position = {v * 10.0f, v * 5.0f, v * 2.0f, 1.0f};
      patterns_[i].velocity = {v, v * 0.5f, v * 0.25f, 0.0f};
      // block_size field removed - not part of PatternData structure
    }
  }

  double measureOperation(std::function<void()> op) {
    auto start = std::chrono::high_resolution_clock::now();
    op();
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double, std::milli>(end - start).count();
  }

  std::unique_ptr<Object> object_;
  std::unique_ptr<Mesh> mesh_;
  std::unique_ptr<MeshHandler> handler_;
  std::vector<sep::pattern::PatternData> patterns_;
};

TEST_F(MeshHandlerPerfTest, PatternUpdatePerformance) {
  double total_time = 0.0;
  int iterations = 100;

  for (int i = 0; i < iterations; ++i) {
    double time =
        measureOperation([this, i]() { handler_->update(patterns_[i % patterns_.size()]); });
    total_time += time;
    EXPECT_LT(time, 16.0) << "Pattern update took too long: " << time << "ms";
  }

  double avg_time = total_time / iterations;
  std::cout << "Average pattern update time: " << avg_time << "ms\n";
  EXPECT_LT(avg_time, 8.0) << "Average pattern update time exceeds target";
}

TEST_F(MeshHandlerPerfTest, DeformationPerformance) {
  MeshHandler::DeformParams params;
  params.strength = 0.5f;
  params.smoothness = 0.3f;
  params.preserve_volume = true;
  params.use_falloff = true;

  double total_time = 0.0;
  int iterations = 100;

  for (int i = 0; i < iterations; ++i) {
    handler_->update(patterns_[i % patterns_.size()]);
    double time = measureOperation([this, &params]() { handler_->applyDeformation(params); });
    total_time += time;
    EXPECT_LT(time, 2.0) << "Deformation took too long: " << time << "ms";
  }

  double avg_time = total_time / iterations;
  std::cout << "Average deformation time: " << avg_time << "ms\n";
  EXPECT_LT(avg_time, 1.0) << "Average deformation time exceeds target";
}

TEST_F(MeshHandlerPerfTest, CustomDataLayerPerformance) {
  double total_time = 0.0;
  int iterations = 10;

  for (int i = 0; i < iterations; ++i) {
    char name[32];
    (void)snprintf(name, sizeof(name), "perf_layer_%d", i);

    double time = measureOperation([this, &name]() {
      handler_->addCustomDataLayer(name, CD_PROP_FLOAT3);
      handler_->update(patterns_[0]);
      handler_->removeCustomDataLayer(name);
    });

    total_time += time;
    EXPECT_LT(time, 5.0) << "Custom data layer operation took too long: " << time << "ms";
  }

  double avg_time = total_time / iterations;
  std::cout << "Average custom data layer operation time: " << avg_time << "ms\n";
  EXPECT_LT(avg_time, 2.0) << "Average custom data layer operation time exceeds target";
}

TEST_F(MeshHandlerPerfTest, MetricsCalculationPerformance) {
  double total_time = 0.0;
  int iterations = 100;

  for (int i = 0; i < iterations; ++i) {
    handler_->update(patterns_[i % patterns_.size()]);
    double time = measureOperation([this]() {
      auto metrics = handler_->getMetrics();
      EXPECT_GT(metrics.surface_area, 0.0f);
    });
    total_time += time;
    EXPECT_LT(time, 1.0) << "Metrics calculation took too long: " << time << "ms";
  }

  double avg_time = total_time / iterations;
  std::cout << "Average metrics calculation time: " << avg_time << "ms\n";
  EXPECT_LT(avg_time, 0.5) << "Average metrics calculation time exceeds target";
}

TEST_F(MeshHandlerPerfTest, MemoryUsage) {
  size_t initial_vertices = mesh_->totvert;
  size_t initial_memory = sizeof(MVert) * initial_vertices;

  // Add custom data layers
  for (int i = 0; i < 5; ++i) {
    char name[32];
    (void)snprintf(name, sizeof(name), "memory_layer_%d", i);
    EXPECT_EQ(sep::SEPResult::SUCCESS, handler_->addCustomDataLayer(name, CD_PROP_FLOAT3));
  }

  auto metrics = handler_->getMetrics();
  EXPECT_GT(metrics.vertex_count, 0u);
  EXPECT_TRUE(metrics.has_custom_data);

  // Memory usage should be reasonable
  size_t expected_memory = initial_memory + (sizeof(float) * 3 * initial_vertices * 5);
  std::cout << "Memory usage: " << expected_memory / 1024 << "KB\n";
  EXPECT_LT(expected_memory, 100 * 1024 * 1024) << "Memory usage exceeds 100MB";
}

