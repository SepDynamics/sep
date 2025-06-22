/**
 * @file mock_mesh_handler.cpp
 * @brief Mock implementation of MeshHandler for testing
 */

#include "blender/mesh_handler.h"
#include <cstring>
#include <memory>
#include <algorithm>

// Mock implementation of Blender-specific functions
extern "C" {
  // These would normally be provided by Blender
  void BKE_mesh_calc_normals(Mesh* mesh) { (void)mesh; }
  void BKE_mesh_batch_cache_dirty_tag(Mesh* mesh, int mode) {
    (void)mesh;
    (void)mode;
  }
  void DEG_id_tag_update(void* id, int flag) {
    (void)id;
    (void)flag;
  }
}

// Constructor
MeshHandler::MeshHandler() 
    : object_(nullptr), mesh_(nullptr), initialized_(false) {
  // Initialize cache
  cache_.metrics_valid = false;
}

// Destructor
MeshHandler::~MeshHandler() {
  // Clean up custom data
  cleanupCustomData();
}

// Initialize with Blender mesh
SEPResult MeshHandler::init(Object* bl_object, Mesh* bl_mesh) {
  if (!bl_object || !bl_mesh) {
    return SEPResult::INVALID_ARGUMENT;
  }
  
  object_ = bl_object;
  mesh_ = bl_mesh;
  initialized_ = true;
  
  // Reset cache
  cache_.metrics_valid = false;
  
  return SEPResult::SUCCESS;
}

// Update mesh based on pattern data
SEPResult MeshHandler::update(const sep::pattern::PatternData& pattern_data) {
  if (!initialized_) {
    return SEPResult::NOT_INITIALIZED;
  }
  
  if (!validatePattern(pattern_data)) {
    return SEPResult::INVALID_ARGUMENT;
  }
  
  // Store pattern coherence for later use
  pattern_state_.coherence = pattern_data.coherence;
  
  // In a real implementation, this would update the mesh vertices
  // For the mock, we'll just invalidate the cache
  cache_.metrics_valid = false;
  
  return SEPResult::SUCCESS;
}

// Get mesh metrics
MeshHandler::MeshMetrics MeshHandler::getMetrics() const {
  MeshMetrics metrics;
  
  if (initialized_) {
    // In a real implementation, this would calculate actual metrics
    // For the mock, we'll just return some reasonable values
    metrics.vertex_count = 100;
    metrics.edge_count = 300;
    metrics.face_count = 200;
    metrics.average_edge_length = 1.0f;
    metrics.surface_area = 100.0f;
    metrics.has_custom_data = !custom_layers_.empty();
  }
  
  return metrics;
}

// Custom data layer management
SEPResult MeshHandler::addCustomDataLayer(const char* name, int type) {
  if (!initialized_) {
    return SEPResult::NOT_INITIALIZED;
  }
  
  if (!name || strlen(name) == 0) {
    return SEPResult::INVALID_ARGUMENT;
  }
  
  // Check if layer already exists
  if (hasCustomDataLayer(name)) {
    return SEPResult::ALREADY_INITIALIZED;
  }
  
  // Create new layer
  CustomDataLayer layer;
  layer.type = type;
  strncpy(layer.name, name, sizeof(layer.name) - 1);
  layer.name[sizeof(layer.name) - 1] = '\0';
  layer.size = getElementSize(type) * 100;  // Assume 100 vertices
  layer.active = true;
  
  // Allocate memory for the layer
  layer.data = std::make_unique<char[]>(layer.size);
  memset(layer.data.get(), 0, layer.size);
  
  // Add to layers
  custom_layers_.push_back(layer);
  
  // Invalidate cache
  cache_.metrics_valid = false;
  
  return SEPResult::SUCCESS;
}

SEPResult MeshHandler::removeCustomDataLayer(const char* name) {
  if (!initialized_) {
    return SEPResult::NOT_INITIALIZED;
  }
  
  if (!name || strlen(name) == 0) {
    return SEPResult::INVALID_ARGUMENT;
  }
  
  // Find layer
  for (auto it = custom_layers_.begin(); it != custom_layers_.end(); ++it) {
    if (strcmp(it->name, name) == 0) {
      // Remove layer
      custom_layers_.erase(it);
      
      // Invalidate cache
      cache_.metrics_valid = false;
      
      return SEPResult::SUCCESS;
    }
  }
  
  return SEPResult::OBJECT_NOT_FOUND;
}

bool MeshHandler::hasCustomDataLayer(const char* name) const {
  if (!initialized_ || !name || strlen(name) == 0) {
    return false;
  }
  
  // Find layer
  for (const auto& layer : custom_layers_) {
    if (strcmp(layer.name, name) == 0) {
      return true;
    }
  }
  
  return false;
}

SEPResult MeshHandler::setUniformFloatLayer(const char* name, float value) {
  if (!initialized_) {
    return SEPResult::NOT_INITIALIZED;
  }
  
  if (!name || strlen(name) == 0) {
    return SEPResult::INVALID_ARGUMENT;
  }
  
  // Find layer
  for (auto& layer : custom_layers_) {
    if (strcmp(layer.name, name) == 0) {
      // Set uniform value
      float* data = reinterpret_cast<float*>(layer.data.get());
      size_t count = layer.size / sizeof(float);
      
      for (size_t i = 0; i < count; ++i) {
        data[i] = value;
      }
      
      return SEPResult::SUCCESS;
    }
  }
  
  return SEPResult::OBJECT_NOT_FOUND;
}

// Pattern-driven deformation
SEPResult MeshHandler::applyDeformation(const DeformParams& params) {
  (void)params;
  if (!initialized_) {
    return SEPResult::NOT_INITIALIZED;
  }
  
  // In a real implementation, this would deform the mesh
  // For the mock, we'll just invalidate the cache
  cache_.metrics_valid = false;
  
  return SEPResult::SUCCESS;
}

// Generate an N-dimensional mesh
SEPResult MeshHandler::generateHyperMesh(const sep::pattern::PatternData& pattern,
                                         int dimensions) {
  if (!initialized_) {
    return SEPResult::NOT_INITIALIZED;
  }
  
  if (dimensions < 3) {
    return SEPResult::INVALID_ARGUMENT;
  }
  
  if (!validatePattern(pattern)) {
    return SEPResult::INVALID_ARGUMENT;
  }
  
  // In a real implementation, this would generate a hypermesh
  // For the mock, we'll just create custom data layers for the extra dimensions
  for (int d = 3; d < dimensions; ++d) {
    char name[64];
    (void)snprintf(name, sizeof(name), "coord_dim%d", d);
    
    // Add custom data layer if it doesn't exist
    if (!hasCustomDataLayer(name)) {
      addCustomDataLayer(name, 0);  // Type 0 = float
    }
  }
  
  // Invalidate cache
  cache_.metrics_valid = false;
  
  return SEPResult::SUCCESS;
}

// Private methods

// Mesh operations
SEPResult MeshHandler::updateVertices(const sep::pattern::PatternData& pattern_data) {
  (void)pattern_data;
  // Mock implementation
  return SEPResult::SUCCESS;
}

SEPResult MeshHandler::updateCustomData(const sep::pattern::PatternData& pattern_data) {
  (void)pattern_data;
  // Mock implementation
  return SEPResult::SUCCESS;
}

SEPResult MeshHandler::notifyDepsgraph() {
  // Mock implementation
  if (mesh_) {
    BKE_mesh_batch_cache_dirty_tag(mesh_, BKE_MESH_BATCH_DIRTY_ALL);
    DEG_id_tag_update(mesh_, ID_RECALC_GEOMETRY);
  }
  return SEPResult::SUCCESS;
}

// Validation
bool MeshHandler::validateMesh() const {
  return initialized_ && mesh_ != nullptr;
}

bool MeshHandler::validatePattern(const sep::pattern::PatternData& pattern_data) const {
  // Basic validation
  return pattern_data.coherence >= 0.0f && pattern_data.coherence <= 1.0f;
}

// Helper methods
void MeshHandler::cleanupCustomData() {
  custom_layers_.clear();
}

void MeshHandler::updateNormals() {
  if (mesh_) {
    BKE_mesh_calc_normals(mesh_);
  }
}

float MeshHandler::calculateSurfaceArea() const {
  // Mock implementation
  return 100.0f;
}

float MeshHandler::calculateAverageEdgeLength() const {
  // Mock implementation
  return 1.0f;
}

void MeshHandler::ensureCustomDataCapacity(size_t vertex_count) {
  (void)vertex_count;
  // Mock implementation
}

// Pattern influence calculation
float MeshHandler::calculateVertexInfluence(
    const sep::pattern::PatternData& pattern,
    const float* vertex) const {
  (void)pattern;
  (void)vertex;
  // Mock implementation
  return 0.5f;
}

void MeshHandler::calculateDisplacement(
    const sep::pattern::PatternData& pattern,
    const float* vertex,
    float* displacement) const {
  (void)pattern;
  (void)vertex;
  // Mock implementation
  displacement[0] = 0.0f;
  displacement[1] = 0.0f;
  displacement[2] = 0.0f;
}

void MeshHandler::updateFloatLayer(
    CustomDataLayer& layer,
    const sep::pattern::PatternData& pattern) {
  (void)layer;
  (void)pattern;
  // Mock implementation
}

void MeshHandler::updateFloat3Layer(
    CustomDataLayer& layer,
    const sep::pattern::PatternData& pattern) {
  (void)layer;
  (void)pattern;
  // Mock implementation
}

float MeshHandler::computeCoherenceWeight(float weight) const {
  // Mock implementation
  return weight * pattern_state_.coherence;
}

size_t MeshHandler::getElementSize(int type) const {
  // Mock implementation
  switch (type) {
    case 0:  // Float
      return sizeof(float);
    case 1:  // Float3
      return sizeof(float) * 3;
    default:
      return sizeof(float);
  }
}