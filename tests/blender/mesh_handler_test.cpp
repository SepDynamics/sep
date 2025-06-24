#include "blender/mesh_handler.h"

#include "../../include/quantum/types.h"
#include <gtest/gtest.h>

#include <cmath>
#include <memory>

#include "blender/compat/blender_types.h"

// Explicitly bring PatternData into scope
using namespace sep::pattern;

class MeshHandlerTest : public ::testing::Test {
 protected:
  void SetUp() override {
    // Create test mesh
    mesh_ = std::make_unique<Mesh>();
    mesh_->totvert = 8;   // Cube vertices
    mesh_->totedge = 12;  // Cube edges
    mesh_->totpoly = 6;   // Cube faces

    // Allocate arrays
    mesh_->mvert = new MVert[mesh_->totvert];
    mesh_->medge = new MEdge[mesh_->totedge];
    mesh_->mloop = new MLoop[mesh_->totpoly * 4];  // 4 vertices per face
    mesh_->mpoly = new MPoly[mesh_->totpoly];

    // Initialize cube vertices
    float vertices[8][3] = {{-1, -1, -1}, {1, -1, -1}, {1, 1, -1}, {-1, 1, -1},
                            {-1, -1, 1},  {1, -1, 1},  {1, 1, 1},  {-1, 1, 1}};
    for (int i = 0; i < 8; ++i) {
      memcpy(mesh_->mvert[i].co, vertices[i], sizeof(float) * 3);
    }

    // Initialize cube edges
    int edges[12][2] = {
        {0, 1}, {1, 2}, {2, 3}, {3, 0},  // Bottom face
        {4, 5}, {5, 6}, {6, 7}, {7, 4},  // Top face
        {0, 4}, {1, 5}, {2, 6}, {3, 7}   // Connecting edges
    };
    for (int i = 0; i < 12; ++i) {
      mesh_->medge[i].v1 = edges[i][0];
      mesh_->medge[i].v2 = edges[i][1];
    }

    // Initialize cube faces
    int faces[6][4] = {
        {0, 1, 2, 3},  // Bottom
        {4, 5, 6, 7},  // Top
        {0, 1, 5, 4},  // Front
        {1, 2, 6, 5},  // Right
        {2, 3, 7, 6},  // Back
        {3, 0, 4, 7}   // Left
    };
    int loop_index = 0;
    for (int i = 0; i < 6; ++i) {
      mesh_->mpoly[i].loopstart = loop_index;
      mesh_->mpoly[i].totloop = 4;
      for (int j = 0; j < 4; ++j) {
        mesh_->mloop[loop_index + j].v = faces[i][j];
      }
      loop_index += 4;
    }

    // Create test object
    object_ = std::make_unique<Object>();
    object_->type = OB_MESH;
    object_->data = mesh_.get();

    // Initialize handler
    handler_ = std::make_unique<MeshHandler>();
    ASSERT_EQ(SEPResult::SUCCESS, handler_->init(object_.get(), mesh_.get()));
  }

  void TearDown() override {
    // Cleanup mesh data
    delete[] mesh_->mvert;
    delete[] mesh_->medge;
    delete[] mesh_->mloop;
    delete[] mesh_->mpoly;
  }

  PatternData createTestPattern() {
    PatternData pattern;
    pattern.coherence = 0.8f;
    pattern.entropy = 0.2f;
    pattern.stability = 0.9f;
    pattern.mutation_rate = 0.1f;
    pattern.generation = 1;
    pattern.position = {0.0f, 0.0f, 0.0f, 1.0f};
    pattern.velocity = {1.0f, 0.0f, 0.0f, 0.0f};
    pattern.size_in_bytes = 96;  // Using size_in_bytes instead of block_size
    return pattern;
  }

  std::unique_ptr<Object> object_;
  std::unique_ptr<Mesh> mesh_;
  std::unique_ptr<MeshHandler> handler_;
};

TEST_F(MeshHandlerTest, InitializationTest) {
  MeshHandler handler;
  EXPECT_EQ(SEPResult::SUCCESS, handler.init(object_.get(), mesh_.get()));
}

TEST_F(MeshHandlerTest, MetricsTest) {
  auto metrics = handler_->getMetrics();
  EXPECT_EQ(8u, metrics.vertex_count);
  EXPECT_EQ(12u, metrics.edge_count);
  EXPECT_EQ(6u, metrics.face_count);
  EXPECT_FLOAT_EQ(24.0f, metrics.surface_area);        // Cube surface area = 24
  EXPECT_FLOAT_EQ(2.0f, metrics.average_edge_length);  // Edge length = 2
}

TEST_F(MeshHandlerTest, CustomDataTest) {
  EXPECT_EQ(SEPResult::SUCCESS, handler_->addCustomDataLayer("test_float", CD_PROP_FLOAT));
  EXPECT_TRUE(handler_->hasCustomDataLayer("test_float"));

  EXPECT_EQ(SEPResult::SUCCESS, handler_->addCustomDataLayer("test_float3", CD_PROP_FLOAT3));
  EXPECT_TRUE(handler_->hasCustomDataLayer("test_float3"));

  EXPECT_EQ(SEPResult::SUCCESS, handler_->removeCustomDataLayer("test_float"));
  EXPECT_FALSE(handler_->hasCustomDataLayer("test_float"));
  EXPECT_TRUE(handler_->hasCustomDataLayer("test_float3"));
}

TEST_F(MeshHandlerTest, PatternUpdateTest) {
  auto pattern = createTestPattern();
  EXPECT_EQ(SEPResult::SUCCESS, handler_->update(pattern));

  // Verify metrics after update
  auto metrics = handler_->getMetrics();
  EXPECT_EQ(8u, metrics.vertex_count);
  EXPECT_TRUE(metrics.has_custom_data);
}

TEST_F(MeshHandlerTest, DeformationTest) {
  auto pattern = createTestPattern();
  EXPECT_EQ(SEPResult::SUCCESS, handler_->update(pattern));

  MeshHandler::DeformParams params;
  params.strength = 0.5f;
  params.smoothness = 0.3f;
  params.preserve_volume = true;
  params.use_falloff = true;

  EXPECT_EQ(SEPResult::SUCCESS, handler_->applyDeformation(params));

  // Verify deformation
  float* v0 = mesh_->mvert[0].co;
  EXPECT_NE(-1.0f, v0[0]);        // Should be deformed
  EXPECT_FLOAT_EQ(-1.0f, v0[1]);  // Y should be unchanged
  EXPECT_FLOAT_EQ(-1.0f, v0[2]);  // Z should be unchanged
}

TEST_F(MeshHandlerTest, InvalidInputTest) {
  // Test null object
  MeshHandler handler;
  EXPECT_EQ(SEPResult::INVALID_OBJECT, handler.init(nullptr, mesh_.get()));

  // Test null mesh
  EXPECT_EQ(SEPResult::INVALID_OBJECT, handler.init(object_.get(), nullptr));

  // Test invalid object type
  object_->type = OB_EMPTY;
  EXPECT_EQ(SEPResult::INVALID_OBJECT, handler.init(object_.get(), mesh_.get()));
}

TEST_F(MeshHandlerTest, PatternValidationTest) {
  PatternData pattern = createTestPattern();
  pattern.coherence = 2.0f;  // Invalid value
  EXPECT_EQ(SEPResult::INVALID_STATE, handler_->update(pattern));

  pattern.coherence = 0.5f;
  pattern.entropy = -1.0f;  // Invalid value
  EXPECT_EQ(SEPResult::INVALID_STATE, handler_->update(pattern));
}

TEST_F(MeshHandlerTest, CustomDataLayerLimitsTest) {
  // Add maximum number of layers
  for (int i = 0; i < 16; ++i) {
    char name[32];
    (void)snprintf(name, sizeof(name), "layer_%d", i);
    EXPECT_EQ(SEPResult::SUCCESS, handler_->addCustomDataLayer(name, CD_PROP_FLOAT));
  }

  // Verify metrics
  auto metrics = handler_->getMetrics();
  EXPECT_TRUE(metrics.has_custom_data);

  // Try to add one more layer
  EXPECT_EQ(SEPResult::INVALID_STATE, handler_->addCustomDataLayer("overflow", CD_PROP_FLOAT));
}

