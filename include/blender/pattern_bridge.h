#ifndef SEP_BLENDER_PATTERN_BRIDGE_H
#define SEP_BLENDER_PATTERN_BRIDGE_H

#include "compat/shim.h"

// Assuming these are the correct paths and necessary includes from the original bridge.h
#include "blender/base_types.h"
#include "blender/compat/blender_types.h"  // If still needed
#include "blender/gpu_context.h"           // For GPUContext
#include "blender/pattern_common.h"  // For ObjectState (assuming)
#include "quantum/data.hpp"
#include "core/types.h"
#include "blender/pattern_observer.h"   // For PatternObserver
#include "blender/types.h"              // For Object, SEPResult, etc.
#include "memory/memory_tier.hpp"   // Provides MemoryTier
#include "memory/memory_tier_manager.hpp"  // For MemoryTierManager

#include <atomic>
#include <map>
#include <memory>
#include <mutex>
#include <thread>


// Forward declaration for PatternProcessor

namespace sep {
namespace pattern {

struct PatternLimits {
    static constexpr std::size_t MAX_PATTERNS = 10000;
    static constexpr float MIN_COHERENCE_VALUE = 0.0f;
    static constexpr float MAX_COHERENCE = 1.0f;
};

// Forward declare if necessary, or include the full definition
class PatternProcessor;

// ObjectState structure definition
struct ObjectState {
    Object* object = nullptr;
    sep::pattern::PatternConfig config;
    PatternStateEnum state = PatternStateEnum::UNINITIALIZED;
    bool needs_update = false;
    bool is_processing = false;
    sep::pattern::PatternMetrics metrics;
    struct {
        size_t host_memory = 0;
        size_t device_memory = 0;
        size_t shared_memory = 0;
    } memory_usage;
    struct {
        float processing_time = 0.0f;
        uint32_t pattern_count = 0;
        uint32_t update_count = 0;
        float coherence_score = 0.0f;
    } stats;
    ::sep::shim::vector<::sep::pattern::PatternData> patterns;
    sep::memory::MemoryBlock* memory_block = nullptr;
};

class BlenderBridge {
 public:
  // Constructor and Destructor
  BlenderBridge();
  ~BlenderBridge();

  // Factory method
  static std::shared_ptr<BlenderBridge> create();

  // Initialization
  sep::SEPResult init(sep::GPUContext* ctx);

  // Object Management
  sep::SEPResult registerObject(Object* obj, const sep::pattern::PatternConfig& config,
                             sep::pattern::ObjectHandle* handle_out);
  sep::SEPResult updateObject(sep::pattern::ObjectHandle handle,
                           const sep::pattern::PatternMetrics& metrics);
  sep::SEPResult cleanupObject(sep::pattern::ObjectHandle handle);  // Added from bridge.cpp context
  bool isValidHandle(sep::pattern::ObjectHandle handle) const;   // Added from bridge.cpp context
  PatternStateEnum getObjectState(
      sep::pattern::ObjectHandle handle) const;  // Added from bridge.cpp context
  
  // Internal helper to get object state pointer
  ObjectState* getObjectStatePtr(sep::pattern::ObjectHandle handle);

  // Pattern Processing
  sep::SEPResult processPatterns();
  sep::SEPResult startProcessingThread();
  sep::SEPResult stopProcessingThread();

  // Memory Management
  sep::SEPResult syncMemory(MemoryTierEnum tier, bool force);
  sep::SEPResult promotePatterns(sep::pattern::ObjectHandle handle, MemoryTierEnum target_tier);
  sep::SEPResult syncPatternData(sep::pattern::ObjectHandle handle, bool force);

  // Observer Management
  void addObserver(std::shared_ptr<sep::pattern::PatternObserver> observer);
  void removeObserver(std::shared_ptr<sep::pattern::PatternObserver> observer);

 private:
  // Internal processing methods
  void processingThreadMain();
  sep::SEPResult processObjectPatterns(sep::pattern::ObjectHandle handle, ObjectState& state);
  sep::SEPResult updatePatternMetrics(ObjectState& state);
  sep::SEPResult validatePatternCoherence(const ObjectState& state);

  // Resource Management
  sep::SEPResult checkResourceLimits();
  sep::SEPResult updateResourceStats();
  float calculateResourceUtilization(sep::pattern::ResourceType type);
  sep::SEPResult checkResourceThresholds();

  // Internal Memory Management helpers
  sep::SEPResult allocatePatternMemory(ObjectState& state);
  sep::SEPResult freePatternMemory(ObjectState& state);

  // Notification helpers
  void notifyObservers(sep::pattern::ObjectHandle handle,
                       const sep::pattern::PatternMetrics& metrics);
  void notifyStateChange(sep::pattern::ObjectHandle handle, PatternStateEnum old_state,
                         PatternStateEnum new_state);
  void notifyError(sep::SEPResult error, const char* message);
  void notifyResourceWarning(sep::pattern::ResourceType type, float utilization);

  // Member Variables
  sep::GPUContext* m_gpu_context = nullptr;
  ::sep::shim::vector<std::shared_ptr<::sep::pattern::PatternObserver>> m_observers;
  std::mutex m_observer_mutex;

  // Map to store object states, keyed by ObjectHandle
  std::map<sep::pattern::ObjectHandle, ObjectState> objects_;
  mutable std::mutex objects_mutex_;
  
  // Object handle generation
  std::atomic<sep::pattern::ObjectHandle> next_handle_{1};

  std::unique_ptr<sep::pattern::PatternProcessor> m_pattern_processor;

  // Threading related
  std::unique_ptr<std::thread> m_processing_thread;
  std::atomic<bool> m_processing_thread_active{false};
};

}  // namespace pattern
}  // namespace sep

#endif  // SEP_BLENDER_PATTERN_BRIDGE_H
