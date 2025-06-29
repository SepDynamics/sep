/**
 * @file mesh_handler.cpp
 * @brief Default MeshHandler implementation used when Blender APIs are absent.
 */

#include "blender/mesh_handler.h"

using namespace sep::pattern;
#include "core/common.h"  // defines sep::SEPResult

using namespace sep::pattern;

// Minimal stand-ins for Blender API functions. These are no-ops here but allow
// the library to link without the real Blender environment.
extern "C" {
  void BKE_mesh_calc_normals(Mesh* /*mesh*/) {}
  void BKE_mesh_batch_cache_dirty_tag(Mesh* /*mesh*/, int /*mode*/) {}
  void DEG_id_tag_update(void* /*id*/, int /*flag*/) {}
}

// ---------------------------------------------------------------------------
// MeshHandler public API
// ---------------------------------------------------------------------------

MeshHandler::MeshHandler()
    : object_(nullptr), mesh_(nullptr), initialized_(false) {
  cache_.metrics_valid = false;
}

MeshHandler::~MeshHandler() { cleanupCustomData(); }

sep::SEPResult MeshHandler::init(Object* bl_object, Mesh* bl_mesh) {
  if (!bl_object || !bl_mesh) {
    return sep::SEPResult::INVALID_ARGUMENT;
  }

  object_ = bl_object;
  mesh_ = bl_mesh;
  initialized_ = true;
  cache_.metrics_valid = false;
  return sep::SEPResult::SUCCESS;
}

sep::SEPResult MeshHandler::update(const sep::pattern::PatternData& pattern_data) {
  if (!initialized_) {
    return sep::SEPResult::INITIALIZATION_FAILED;
  }

  if (!validatePattern(pattern_data)) {
    return sep::SEPResult::INVALID_ARGUMENT;
  }

  pattern_state_.coherence = pattern_data.coherence;

  sep::SEPResult res = updateVertices(pattern_data);
  if (res != sep::SEPResult::SUCCESS) {
    return res;
  }

  res = updateCustomData(pattern_data);
  if (res != sep::SEPResult::SUCCESS) {
    return res;
  }

  updateNormals();
  cache_.metrics_valid = false;
  return sep::SEPResult::SUCCESS;
}

MeshHandler::MeshMetrics MeshHandler::getMetrics() const {
  MeshMetrics metrics;
  if (initialized_) {
    if (!cache_.metrics_valid) {
      cache_.metrics.vertex_count = static_cast<size_t>(mesh_->totvert);
      cache_.metrics.edge_count = static_cast<size_t>(mesh_->totedge);
      cache_.metrics.face_count = static_cast<size_t>(mesh_->totpoly);
      cache_.metrics.average_edge_length = calculateAverageEdgeLength();
      cache_.metrics.surface_area = calculateSurfaceArea();
      cache_.metrics.has_custom_data = !custom_layers_.empty();
      cache_.metrics_valid = true;
    }
    metrics = cache_.metrics;
  }
  return metrics;
}

sep::SEPResult MeshHandler::addCustomDataLayer(const char* name, int type) {
  if (!initialized_) {
    return sep::SEPResult::INITIALIZATION_FAILED;
  }

  if (!name || std::strlen(name) == 0) {
    return sep::SEPResult::INVALID_ARGUMENT;
  }

  if (hasCustomDataLayer(name)) {
    return sep::SEPResult::ALREADY_EXISTS;
  }

  if (custom_layers_.size() >= 16) {
    return sep::SEPResult::INVALID_STATE;
  }

  CustomDataLayer layer;
  layer.type = type;
  std::strncpy(layer.name, name, sizeof(layer.name) - 1);
  layer.name[sizeof(layer.name) - 1] = '\0';
  size_t vert_count = static_cast<size_t>(mesh_ ? mesh_->totvert : 0);
  if (vert_count == 0)
    vert_count = 1;
  layer.size = getElementSize(type) * vert_count;
  layer.active = true;

  layer.data = std::make_unique<char[]>(layer.size);
  std::memset(layer.data.get(), 0, layer.size);

  custom_layers_.push_back(layer);
  cache_.metrics_valid = false;
  return sep::SEPResult::SUCCESS;
}

sep::SEPResult MeshHandler::removeCustomDataLayer(const char* name) { 
  if (!initialized_) {
    return sep::SEPResult::INITIALIZATION_FAILED;
  }

  if (!name || std::strlen(name) == 0) {
    return sep::SEPResult::INVALID_ARGUMENT;
  }

  for (auto it = custom_layers_.begin(); it != custom_layers_.end(); ++it) {
    if (std::strcmp(it->name, name) == 0) {
      custom_layers_.erase(it);
      cache_.metrics_valid = false;
      return sep::SEPResult::SUCCESS;
    }
  }

  return sep::SEPResult::NOT_FOUND;
}

bool MeshHandler::hasCustomDataLayer(const char* name) const {
  if (!initialized_ || !name || std::strlen(name) == 0) {
    return false;
  }

  for (const auto& layer : custom_layers_) {
    if (std::strcmp(layer.name, name) == 0) {
      return true;
    }
  }
  return false;
}

sep::SEPResult MeshHandler::setUniformFloatLayer(const char* name, float value) {
  if (!initialized_) {
    return sep::SEPResult::INITIALIZATION_FAILED;
  }

  if (!name || std::strlen(name) == 0) {
    return sep::SEPResult::INVALID_ARGUMENT;
  }

  for (auto& layer : custom_layers_) {
    if (std::strcmp(layer.name, name) == 0) {
      float* data = reinterpret_cast<float*>(layer.data.get());
      size_t count = layer.size / sizeof(float);
      for (size_t i = 0; i < count; ++i) {
        data[i] = value;
      }
      return sep::SEPResult::SUCCESS;
    }
  }

  return sep::SEPResult::NOT_FOUND;
}

sep::SEPResult MeshHandler::applyDeformation(const DeformParams& params) {
  if (!initialized_) {
    return sep::SEPResult::INITIALIZATION_FAILED;
  }

  if (pattern_state_.weights.empty()) {
    // No pattern data applied yet; nothing to deform
    return sep::SEPResult::SUCCESS;
  }

  for (int i = 0; i < mesh_->totvert; ++i) {
    float* v = mesh_->mvert[i].co;
    float d = params.strength * pattern_state_.weights[i];
    v[0] += d;
  }

  updateNormals();
  cache_.metrics_valid = false;
  return notifyDepsgraph();
}

sep::SEPResult MeshHandler::generateHyperMesh(const sep::pattern::PatternData& pattern,
                                         int dimensions) {
  if (!initialized_) {
    return sep::SEPResult::INITIALIZATION_FAILED;
  }
  if (dimensions < 3 || !validatePattern(pattern)) {
    return sep::SEPResult::INVALID_ARGUMENT;
  }

  for (int d = 3; d < dimensions; ++d) {
    char name[64];
    (void)std::snprintf(name, sizeof(name), "coord_dim%d", d);
    if (!hasCustomDataLayer(name)) {
      addCustomDataLayer(name, 0);  // float layer
    }
  }

  cache_.metrics_valid = false;
  return sep::SEPResult::SUCCESS;
}

// ---------------------------------------------------------------------------
// Private helpers
// ---------------------------------------------------------------------------

sep::SEPResult MeshHandler::updateVertices(const sep::pattern::PatternData& pattern_data) {
  if (!validateMesh()) {
    return sep::SEPResult::INVALID_STATE;
  }

  pattern_state_.weights.resize(static_cast<size_t>(mesh_->totvert));

  for (int i = 0; i < mesh_->totvert; ++i) {
    float* v = mesh_->mvert[i].co;
    float w = calculateVertexInfluence(pattern_data, v);
    pattern_state_.weights[i] = w;
  }

  return sep::SEPResult::SUCCESS;
}

sep::SEPResult MeshHandler::updateCustomData(const sep::pattern::PatternData& pattern_data) {
  if (custom_layers_.empty()) {
    (void)addCustomDataLayer("pattern_weight", 0);
  }

  ensureCustomDataCapacity(static_cast<size_t>(mesh_ ? mesh_->totvert : 0));

  for (auto& layer : custom_layers_) {
    if (layer.type == 0) {
      updateFloatLayer(layer, pattern_data);
    } else if (layer.type == 1) {
      updateFloat3Layer(layer, pattern_data);
    }
  }

  return sep::SEPResult::SUCCESS;
}

sep::SEPResult MeshHandler::notifyDepsgraph() {
  if (mesh_) {
    BKE_mesh_batch_cache_dirty_tag(mesh_, BKE_MESH_BATCH_DIRTY_ALL);
    DEG_id_tag_update(mesh_, ID_RECALC_GEOMETRY);
  }
  return sep::SEPResult::SUCCESS;
}

bool MeshHandler::validateMesh() const { return initialized_ && mesh_ != nullptr; }

bool MeshHandler::validatePattern(const sep::pattern::PatternData& pattern_data) const {
  return pattern_data.coherence >= 0.0f && pattern_data.coherence <= 1.0f;
}

void MeshHandler::cleanupCustomData() { custom_layers_.clear(); }

void MeshHandler::updateNormals() {
  if (mesh_) {
    BKE_mesh_calc_normals(mesh_);
  }
}

float MeshHandler::calculateSurfaceArea() const {
  if (!mesh_ || mesh_->totpoly == 0) {
    return 0.0f;
  }

  double area = 0.0;
  for (int i = 0; i < mesh_->totpoly; ++i) {
    const MPoly& poly = mesh_->mpoly[i];
    if (poly.totloop < 3) {
      continue;
    }
    const float* v0 = mesh_->mvert[mesh_->mloop[poly.loopstart].v].co;
    for (int j = 1; j < poly.totloop - 1; ++j) {
      const float* v1 =
          mesh_->mvert[mesh_->mloop[poly.loopstart + j].v].co;
      const float* v2 =
          mesh_->mvert[mesh_->mloop[poly.loopstart + j + 1].v].co;

      float ax = v1[0] - v0[0];
      float ay = v1[1] - v0[1];
      float az = v1[2] - v0[2];
      float bx = v2[0] - v0[0];
      float by = v2[1] - v0[1];
      float bz = v2[2] - v0[2];
      float cx = ay * bz - az * by;
      float cy = az * bx - ax * bz;
      float cz = ax * by - ay * bx;
      area += 0.5 * std::sqrt(cx * cx + cy * cy + cz * cz);
    }
  }
  return static_cast<float>(area);
}

float MeshHandler::calculateAverageEdgeLength() const {
  if (!mesh_ || mesh_->totedge == 0) {
    return 0.0f;
  }

  double total = 0.0;
  for (int i = 0; i < mesh_->totedge; ++i) {
    const MEdge& e = mesh_->medge[i];
    const float* v1 = mesh_->mvert[e.v1].co;
    const float* v2 = mesh_->mvert[e.v2].co;
    float dx = v1[0] - v2[0];
    float dy = v1[1] - v2[1];
    float dz = v1[2] - v2[2];
    total += std::sqrt(dx * dx + dy * dy + dz * dz);
  }
  return static_cast<float>(total / mesh_->totedge);
}

void MeshHandler::ensureCustomDataCapacity(size_t vertex_count) {
  if (vertex_count == 0) {
    return;
  }

  for (auto& layer : custom_layers_) {
    size_t required = getElementSize(layer.type) * vertex_count;
    if (layer.size != required) {
      layer.data.reset();
      layer.data = std::make_unique<char[]>(required);
      std::memset(layer.data.get(), 0, required);
      layer.size = required;
    }
  }
}

float MeshHandler::calculateVertexInfluence(const sep::pattern::PatternData& pattern,
                                           const float* vertex) const {
  float dx = vertex[0] - pattern.position.x;
  float dy = vertex[1] - pattern.position.y;
  float dz = vertex[2] - pattern.position.z;
  float dist = std::sqrt(dx * dx + dy * dy + dz * dz);
  float weight = 1.0f / (1.0f + dist);
  return computeCoherenceWeight(weight);
}

void MeshHandler::calculateDisplacement(const sep::pattern::PatternData& pattern,
                                       const float* vertex,
                                       float* displacement) const {
  float w = calculateVertexInfluence(pattern, vertex);
  displacement[0] = pattern.velocity.x * w;
  displacement[1] = pattern.velocity.y * w;
  displacement[2] = pattern.velocity.z * w;
}

void MeshHandler::updateFloatLayer(CustomDataLayer& layer,
                                  const sep::pattern::PatternData& pattern) {
  float* data = reinterpret_cast<float*>(layer.data.get());
  for (int i = 0; i < mesh_->totvert; ++i) {
    const float* v = mesh_->mvert[i].co;
    data[i] = calculateVertexInfluence(pattern, v);
  }
}

void MeshHandler::updateFloat3Layer(CustomDataLayer& layer,
                                   const sep::pattern::PatternData& pattern) {
  float* data = reinterpret_cast<float*>(layer.data.get());
  for (int i = 0; i < mesh_->totvert; ++i) {
    const float* v = mesh_->mvert[i].co;
    float disp[3];
    calculateDisplacement(pattern, v, disp);
    data[i * 3 + 0] = disp[0];
    data[i * 3 + 1] = disp[1];
    data[i * 3 + 2] = disp[2];
  }
}

float MeshHandler::computeCoherenceWeight(float weight) const {
  return weight * pattern_state_.coherence;
}

size_t MeshHandler::getElementSize(int type) const {
  switch (type) {
    case 0:
      return sizeof(float);
    case 1:
      return sizeof(float) * 3;
    default:
      return sizeof(float);
  }
}
