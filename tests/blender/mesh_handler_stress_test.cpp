#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <memory>
#include <thread>
#include <vector>

#include "blender/compat/blender_types.h"
#include "blender/mesh_handler.h"
#include "quantum/types.h"

using namespace sep::pattern;

class MeshHandlerStressTest : public ::testing::Test {
 protected:
  void SetUp() override {
    setupMassiveMesh();
    setupPatterns();
  }

  void TearDown() override {
    delete[] mesh_->mvert;
    delete[] mesh_->medge;
    delete[] mesh_->mloop;
    delete[] mesh_->mpoly;
  }

  void setupMassiveMesh() {
    // Create massive test mesh (1000x1000 grid = 1M vertices)
    const int GRID_SIZE = 1000;
    mesh_ = std::make_unique<Mesh>();
    mesh_->totvert = GRID_SIZE * GRID_SIZE;
    mesh_->totedge = (GRID_SIZE - 1) * GRID_SIZE * 2;
    mesh_->totpoly = (GRID_SIZE - 1) * (GRID_SIZE - 1);

    // Allocate arrays
    mesh_->mvert = new MVert[mesh_->totvert];
    mesh_->medge = new MEdge[mesh_->totedge];
    mesh_->mloop = new MLoop[mesh_->totpoly * 4];
    mesh_->mpoly = new MPoly[mesh_->totpoly];

    // Initialize grid vertices with deterministic noise
    float seed = 1.0f;
    auto noise = [&seed]() {
      seed = std::fmod(seed * 1.3f, 1.0f);
      return (seed - 0.5f) * 0.2f;
    };

    for (int i = 0; i < GRID_SIZE; ++i) {
      for (int j = 0; j < GRID_SIZE; ++j) {
        int idx = i * GRID_SIZE + j;
        mesh_->mvert[idx].co[0] = static_cast<float>(i) + noise();
        mesh_->mvert[idx].co[1] = static_cast<float>(j) + noise();
        mesh_->mvert[idx].co[2] = noise();
      }
    }

    // Initialize grid topology
    setupGridTopology(GRID_SIZE);

    // Create test object
    object_ = std::make_unique<Object>();
    object_->type = OB_MESH;
    object_->data = mesh_.get();

    // Initialize handler
    handler_ = std::make_unique<MeshHandler>();
    ASSERT_EQ(sep::SEPResult::SUCCESS, handler_->init(object_.get(), mesh_.get()));
  }

  void setupGridTopology(int GRID_SIZE) {
    // Initialize edges
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

    // Initialize faces
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
  }

  void setupPatterns() {
    patterns_.resize(1000);  // 1000 patterns for stress testing
    for (size_t i = 0; i < patterns_.size(); ++i) {
      float v = static_cast<float>(i) / static_cast<float>(patterns_.size());
      patterns_[i].coherence = v;
      patterns_[i].entropy = v * 0.5f;
      patterns_[i].stability = 1.0f - v;
      patterns_[i].mutation_rate = v * 0.1f;
      patterns_[i].generation = 1;
      patterns_[i].position = {v * 100.0f, v * 50.0f, v * 25.0f, 1.0f};
      patterns_[i].velocity = {v, v * 0.5f, v * 0.25f, 0.0f};
      // block_size field removed - not part of PatternData structure
    }
  }

  std::unique_ptr<Object> object_;
  std::unique_ptr<Mesh> mesh_;
  std::unique_ptr<MeshHandler> handler_;
  std::vector<sep::pattern::PatternData> patterns_;
};

TEST_F(MeshHandlerStressTest, MassiveUpdateTest) {
  const int ITERATIONS = 1000;
  std::atomic<int> success_count{0};
  std::atomic<int> error_count{0};

  auto start = std::chrono::high_resolution_clock::now();

  // OpenMP pragma removed to avoid compiler warnings
  for (int i = 0; i < ITERATIONS; ++i) {
    sep::SEPResult result = handler_->update(patterns_[i % patterns_.size()]);
    if (result == sep::SEPResult::SUCCESS) {
      success_count++;
    } else {
      error_count++;
    }
  }

  auto end = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration<double>(end - start).count();

  std::cout << "Massive update test results:\n"
            << "Total updates: " << ITERATIONS << "\n"
            << "Successful: " << success_count << "\n"
            << "Failed: " << error_count << "\n"
            << "Time taken: " << duration << "s\n"
            << "Updates per second: " << ITERATIONS / duration << "\n";

  EXPECT_GT(success_count, ITERATIONS * 0.95) << "Too many failed updates";
  EXPECT_LT(duration, 60.0) << "Test took too long";
}

TEST_F(MeshHandlerStressTest, ConcurrentOperationsTest) {
  const int THREAD_COUNT = 8;
  const int OPS_PER_THREAD = 100;
  std::vector<std::thread> threads;
  std::atomic<int> errors{0};

  auto worker = [this, &errors](int thread_id) {
    size_t pattern_idx = 0;

    for (int i = 0; i < OPS_PER_THREAD; ++i) {
      // Mix of operations
      pattern_idx = (pattern_idx + thread_id + 1) % patterns_.size();
      switch (i % 4) {
        case 0: {
          // Update pattern
          if (handler_->update(patterns_[pattern_idx]) != sep::SEPResult::SUCCESS) {
            errors++;
          }
          break;
        }
        case 1: {
          // Add/remove custom data
          char name[32];
          (void)snprintf(name, sizeof(name), "stress_layer_%d_%d", thread_id, i);
          if (handler_->addCustomDataLayer(name, CD_PROP_FLOAT) == sep::SEPResult::SUCCESS) {
            handler_->removeCustomDataLayer(name);
          }
          break;
        }
        case 2: {
          // Get metrics
          handler_->getMetrics();
          break;
        }
        case 3: {
          // Apply deformation
          MeshHandler::DeformParams params;
          params.strength = 0.1f;
          params.smoothness = 0.3f;
          if (handler_->applyDeformation(params) != sep::SEPResult::SUCCESS) {
            errors++;
          }
          break;
        }
      }
    }
  };

  auto start = std::chrono::high_resolution_clock::now();

  // Launch threads
  for (int i = 0; i < THREAD_COUNT; ++i) {
    threads.emplace_back(worker, i);
  }

  // Wait for completion
  for (auto& thread : threads) {
    thread.join();
  }

  auto end = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration<double>(end - start).count();

  std::cout << "Concurrent operations test results:\n"
            << "Total operations: " << THREAD_COUNT * OPS_PER_THREAD << "\n"
            << "Errors: " << errors << "\n"
            << "Time taken: " << duration << "s\n"
            << "Operations per second: " << (THREAD_COUNT * OPS_PER_THREAD) / duration << "\n";

  EXPECT_LT(errors, 10) << "Too many errors during concurrent operations";
  EXPECT_LT(duration, 30.0) << "Test took too long";
}

TEST_F(MeshHandlerStressTest, MemoryStressTest) {
  const int LAYER_COUNT = 50;  // Try to add many custom data layers
  std::vector<std::string> layer_names;
  int successful_layers = 0;

  // Monitor initial memory state
  auto initial_metrics = handler_->getMetrics();

  // Try to add many custom data layers
  for (int i = 0; i < LAYER_COUNT; ++i) {
    char name[32];
    (void)snprintf(name, sizeof(name), "stress_memory_%d", i);

    if (handler_->addCustomDataLayer(name, CD_PROP_FLOAT3) == sep::SEPResult::SUCCESS) {
      layer_names.push_back(name);
      successful_layers++;
    }
  }

  // Update patterns with all layers
  for (int i = 0; i < 10; ++i) {
    ASSERT_EQ(sep::SEPResult::SUCCESS, handler_->update(patterns_[i]));
  }

  // Get final metrics
  auto final_metrics = handler_->getMetrics();

  // Cleanup
  for (const auto& name : layer_names) {
    handler_->removeCustomDataLayer(name.c_str());
  }

  std::cout << "Memory stress test results:\n"
            << "Successful layers: " << successful_layers << "/" << LAYER_COUNT << "\n"
            << "Initial vertex count: " << initial_metrics.vertex_count << "\n"
            << "Final vertex count: " << final_metrics.vertex_count << "\n";

  EXPECT_GT(successful_layers, LAYER_COUNT / 2) << "Too few layers created successfully";
  EXPECT_EQ(initial_metrics.vertex_count, final_metrics.vertex_count) << "Vertex count changed";
}

TEST_F(MeshHandlerStressTest, PatternEvolutionStressTest) {
  const int GENERATIONS = 100;
  std::vector<sep::pattern::PatternData> evolving_patterns = patterns_;

  auto start = std::chrono::high_resolution_clock::now();

  for (int gen = 0; gen < GENERATIONS; ++gen) {
    // Update all patterns
    for (auto& pattern : evolving_patterns) {
      // Evolve pattern
      pattern.generation++;
      pattern.coherence *= (1.0f - pattern.entropy);
      pattern.stability *= (1.0f - pattern.mutation_rate);

      // Apply to mesh
      ASSERT_EQ(sep::SEPResult::SUCCESS, handler_->update(pattern));
    }

    // Apply deformation
    MeshHandler::DeformParams params;
    params.strength = 0.1f;
    params.smoothness = 0.3f;
    ASSERT_EQ(sep::SEPResult::SUCCESS, handler_->applyDeformation(params));
  }

  auto end = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration<double>(end - start).count();

  std::cout << "Pattern evolution stress test results:\n"
            << "Generations: " << GENERATIONS << "\n"
            << "Patterns per generation: " << evolving_patterns.size() << "\n"
            << "Time taken: " << duration << "s\n"
            << "Generations per second: " << GENERATIONS / duration << "\n";

  EXPECT_LT(duration, 120.0) << "Evolution test took too long";
}

