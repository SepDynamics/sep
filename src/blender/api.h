#pragma once

#include "blender/types.h"
#include "blender/config.h"
#include "memory/types.h"
#include <cstddef>

#ifdef __cplusplus
namespace sep {
struct SEPBlenderBridge;
}
using SEPBlenderBridge = sep::SEPBlenderBridge;
extern "C" {
#endif

// Forward declarations
struct GPUContext;
struct Object;
struct Mesh;
typedef uint64_t SEPMeshHandle;

// Initialize bridge with Blender GPU context
sep::SEPResult sep_blender_init(
    GPUContext* gpu_ctx,           // [in] Blender GPU context
    const SEPConfig* config,       // [in] SEP configuration
    SEPBlenderBridge** bridge_out  // [out] Bridge instance
);

// Register mesh for pattern monitoring
sep::SEPResult sep_register_mesh(
    SEPBlenderBridge* bridge,      // [in] Bridge instance
    Object* bl_object,             // [in] Blender object (lifetime: caller)
    Mesh* bl_mesh,                 // [in] Mesh data (lifetime: caller)
    SEPMeshHandle* handle_out      // [out] Monitoring handle
);

// Update mesh based on pattern analysis
sep::SEPResult sep_update_mesh(
    SEPBlenderBridge* bridge,      // [in] Bridge instance
    SEPMeshHandle handle,          // [in] Mesh handle
    const SEPPatternMetrics* data, // [in] Pattern data
    bool* updated_out             // [out] True if mesh was modified
);

// Process audio input for voice-driven updates
sep::SEPResult sep_process_audio(
    SEPBlenderBridge* bridge,      // [in] Bridge instance
    const float* samples,          // [in] Audio samples
    size_t count,                  // [in] Sample count
    SEPAudioMetrics* metrics_out   // [out] Processing metrics
);

// Synchronize SEP memory tiers with Blender
sep::SEPResult sep_sync_memory(
    SEPBlenderBridge* bridge,      // [in] Bridge instance
    ::sep::memory::MemoryTierEnum tier,      // [in] Target memory tier
    bool force                     // [in] Force immediate sync
);

// Clean up bridge instance
sep::SEPResult sep_blender_cleanup(
    SEPBlenderBridge* bridge       // [in] Bridge instance to clean up
);

// Get current bridge metrics
sep::SEPResult sep_get_metrics(
    SEPBlenderBridge* bridge,      // [in] Bridge instance
    SEPPatternMetrics* metrics_out // [out] Current metrics
);

// Reset bridge state
sep::SEPResult sep_reset_state(
    SEPBlenderBridge* bridge       // [in] Bridge instance to reset
);

// Version information
const char* sep_get_version(void);
const char* sep_get_build_info(void);

#ifdef __cplusplus
}
#endif
