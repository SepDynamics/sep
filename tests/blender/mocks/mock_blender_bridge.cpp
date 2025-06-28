/**
 * @file mock_blender_bridge.cpp
 * @brief Mock implementation of BlenderBridge for testing
 *
 * This file provides mock implementations of CUDA-dependent functions
 * for testing purposes.
 */

#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

#include "blender/pattern_bridge.h"
#include "blender/types.h"
#include "core/common.h"

namespace sep {
namespace pattern {

// Implementation of startProcessingThread
void BlenderBridge::startProcessingThread() {
    thread_running_ = true;
    // In a mock implementation, we don't actually start a thread
}

// Implementation of stopProcessingThread
void BlenderBridge::stopProcessingThread() {
    thread_running_ = false;
    // In a mock implementation, we don't actually stop a thread
}

// Implementation of processingThreadMain
void BlenderBridge::processingThreadMain() {
    // In a mock implementation, this is empty
}

// Implementation of updateObject
sep::SEPResult BlenderBridge::updateObject(ObjectHandle handle, const PatternMetrics& metrics) {
    (void)handle;
    (void)metrics;
    if (!initialized_) {
        return sep::SEPResult::NOT_INITIALIZED;
    }

    // In a mock implementation, we just return success
    return sep::SEPResult::SUCCESS;
}

// Implementation of processPatterns
sep::SEPResult BlenderBridge::processPatterns() {
    if (!initialized_) {
        return sep::SEPResult::NOT_INITIALIZED;
    }
    
    // In a mock implementation, we just return success
    return sep::SEPResult::SUCCESS;
}

// Implementation of updateResourceStats
void BlenderBridge::updateResourceStats() {
    // In a mock implementation, this is empty
}

}  // namespace pattern
}  // namespace sep

// Define the sep_cuda_init function
extern "C" {
    bool sep_cuda_init(int device_id) {
        (void)device_id;
        // Mock implementation
        return true;
    }
}