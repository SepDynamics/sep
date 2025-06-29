#pragma once

#include "compat/shim.h"

// Assuming these are the correct paths and necessary includes from the original bridge.h
#include "blender/base_types.h"
#include "blender/blender_types.h"  // If still needed
#include "blender/gpu_context.h"           // For GPUContext
#include "blender/pattern_common.h"  // For ObjectState (assuming)
#include "blender/bridge.h"
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


}  // namespace pattern
}  // namespace sep
