#pragma once

#include <cstring>
#include <memory>
#include "compat/shim.h"

#include "blender/compat/blender_types.h"
#include "blender/pattern_common.h"

#include "blender/types.h"
#include "core/common.h"  // for sep::SEPResult

// Use the SEPResult from sep namespace
using sep::SEPResult;

// Blender mesh batch flags
#define BKE_MESH_BATCH_DIRTY_ALL (1 << 0)
#define ID_RECALC_GEOMETRY (1 << 1)

namespace sep::blender {
class PatternVisualizationPipeline;
}
class MeshHandler {
 public:
  MeshHandler();
  ~MeshHandler();

  // Initialize with Blender mesh
  SEPResult init(Object* bl_object, Mesh* bl_mesh);

  // Update mesh based on pattern data
  SEPResult update(const sep::pattern::PatternData& pattern_data);

  // Get mesh metrics
  struct MeshMetrics {
    size_t vertex_count;
    size_t edge_count;
    size_t face_count;
    float average_edge_length;
    float surface_area;
    bool has_custom_data;

    MeshMetrics()
        : vertex_count(0),
          edge_count(0),
          face_count(0),
          average_edge_length(0.0f),
          surface_area(0.0f),
          has_custom_data(false) {}
  };
  MeshMetrics getMetrics() const;

  // Custom data layer management
  SEPResult addCustomDataLayer(const char* name, int type);
  SEPResult removeCustomDataLayer(const char* name);
  bool hasCustomDataLayer(const char* name) const;
  SEPResult setUniformFloatLayer(const char* name, float value);

  // Pattern-driven deformation
  struct DeformParams {
    float strength;        // Deformation strength (0-1)
    float smoothness;      // Smoothing factor (0-1)
    bool preserve_volume;  // Maintain mesh volume
    bool use_falloff;      // Use distance-based falloff

    DeformParams() : strength(1.0f), smoothness(0.5f), preserve_volume(true), use_falloff(true) {}
  };
  SEPResult applyDeformation(const DeformParams& params);

  // Generate an N-dimensional mesh. For dimensions greater than 3 the extra
  // coordinates are stored in custom data layers named "coord_dimX".
  SEPResult generateHyperMesh(const sep::pattern::PatternData& pattern,
                             int dimensions);

 private:
  friend class sep::blender::PatternVisualizationPipeline;
  // Blender data
  Object* object_;
  Mesh* mesh_;
  bool initialized_;

  // Mesh operations
  SEPResult updateVertices(const sep::pattern::PatternData& pattern_data);
  SEPResult updateCustomData(const sep::pattern::PatternData& pattern_data);
  SEPResult notifyDepsgraph();

  // Validation
  bool validateMesh() const;
  bool validatePattern(const sep::pattern::PatternData& pattern_data) const;

  // Custom data layers
  struct CustomDataLayer {
    int type;
    std::unique_ptr<char[]> data;
    char name[64];
    size_t size;
    bool active;

    CustomDataLayer() : type(0), data(nullptr), size(0), active(false) { name[0] = '\0'; }

    CustomDataLayer(const CustomDataLayer& other)
        : type(other.type), data(nullptr), size(other.size), active(other.active) {
      memcpy(name, other.name, sizeof(name));
      if (other.data && other.size > 0) {
        data = std::make_unique<char[]>(other.size);
        memcpy(data.get(), other.data.get(), other.size);
      }
    }

    CustomDataLayer& operator=(const CustomDataLayer& other) {
      if (this != &other) {
        data.reset();
        type = other.type;
        size = other.size;
        active = other.active;
        memcpy(name, other.name, sizeof(name));
        if (other.data && other.size > 0) {
          data = std::make_unique<char[]>(other.size);
          memcpy(data.get(), other.data.get(), other.size);
        }
      }
      return *this;
    }

    CustomDataLayer(CustomDataLayer&& other) noexcept
        : type(other.type), data(std::move(other.data)), size(other.size), active(other.active) {
      memcpy(name, other.name, sizeof(name));
      other.data.reset();
    }

    CustomDataLayer& operator=(CustomDataLayer&& other) noexcept {
      if (this != &other) {
        data.reset();
        type = other.type;
        data = std::move(other.data);
        size = other.size;
        active = other.active;
        memcpy(name, other.name, sizeof(name));
      }
      return *this;
    }

    ~CustomDataLayer() = default;
  };
  ::sep::shim::vector<CustomDataLayer> custom_layers_;

  // Helper methods
  void cleanupCustomData();
  void updateNormals();
  float calculateSurfaceArea() const;
  float calculateAverageEdgeLength() const;
  void ensureCustomDataCapacity(size_t vertex_count);

  // Pattern influence calculation
  float calculateVertexInfluence(const sep::pattern::PatternData& pattern,
                                 const float* vertex) const;
  void calculateDisplacement(const sep::pattern::PatternData& pattern, const float* vertex,
                             float* displacement) const;
  void updateFloatLayer(CustomDataLayer& layer, const sep::pattern::PatternData& pattern);
  void updateFloat3Layer(CustomDataLayer& layer, const sep::pattern::PatternData& pattern);
  float computeCoherenceWeight(float weight) const;
  size_t getElementSize(int type) const;

  // Pattern processing
  struct PatternState {
    ::sep::shim::vector<float> weights;
    ::sep::shim::vector<float> displacements;
    ::sep::shim::vector<bool> affected_vertices;
    float coherence{0.0f};

    PatternState() = default;
  };
  PatternState pattern_state_;

  // Cache for optimization
  struct {
    mutable MeshMetrics metrics;
    mutable bool metrics_valid;
    ::sep::shim::vector<size_t> vertex_map;
    ::sep::shim::vector<size_t> edge_map;
  } cache_;

  // Prevent copying
  MeshHandler(const MeshHandler&) = delete;
  MeshHandler& operator=(const MeshHandler&) = delete;
};
