#pragma once

#include "blender/base_types.h"  // For PatternMetrics, PatternState, SEPResult
#include "core/types.h"  // For PatternStateEnum

namespace sep {
namespace pattern {

class PatternObserver {
public:
    virtual ~PatternObserver() = default;

    // Called when pattern metrics are updated
    virtual void onPatternUpdate(ObjectHandle handle, const PatternMetrics& metrics) = 0;

    // Called when pattern state changes
    virtual void onStateChange(ObjectHandle handle, PatternStateEnum old_state, PatternStateEnum new_state) = 0;

    // Called when an error occurs
    virtual void onError(SEPResult error, const char* message) = 0;

    // Called when resource utilization exceeds thresholds
    virtual void onResourceWarning(ResourceType type, float utilization) = 0;
};

} // namespace pattern
} // namespace sep
