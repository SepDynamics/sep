#include <cstdlib>
#include <cstring>
#include <ctime>

#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <memory>
#include <new>
#include <sstream>
#include <string>
#include <thread>
#include <utility>
#include <vector>
#include <unistd.h>

#include "blender/pattern_bridge.h"
#include "quantum/processor.h"
#include "quantum/data.hpp"
#include "blender/pattern_observer.h"
#include "quantum/types.h"
#include "memory/types.h"

using ::sep::memory::MemoryTierEnum;
#include "core/common.h"  // For sep::SEPResult

#include "memory/memory_tier_manager.hpp"

namespace sep {
namespace pattern {
// Use the core SEPResult enum instead of the pattern-specific one
// Constructor implementation - Initialize members in the correct order
BlenderBridge::BlenderBridge()
    : thread_running_(false),
      gpu_context_(nullptr)
{
    // Initialize the bridge
}

BlenderBridge::~BlenderBridge() = default;


std::unique_ptr<BlenderBridge> BlenderBridge::create()
{
    return std::unique_ptr<BlenderBridge>(new (std::nothrow) BlenderBridge()); 
} 


sep::SEPResult BlenderBridge::init(sep::GPUContext* ctx)
{
    if (!ctx)
    {
        return sep::SEPResult::INVALID_ARGUMENT;
    }

    if (thread_running_.load())
    {
        return sep::SEPResult::ALREADY_EXISTS;
    }

    // Store GPU context
    gpu_context_ = ctx;

    // Initialize pattern processor
    pattern_processor_.reset(new (std::nothrow) sep::pattern::PatternProcessor());
    if (!pattern_processor_)
    {
        notifyError(sep::SEPResult::ALLOCATION_FAILED, "Failed to allocate pattern processor");
        return sep::SEPResult::ALLOCATION_FAILED;
    }

    // No initialization needed on PatternProcessor

    // Start processing thread
    startProcessingThread();

    return sep::SEPResult::SUCCESS;
}

sep::SEPResult BlenderBridge::registerObject(Object* obj, const PatternConfig& config, ObjectHandle* handle_out)
{
    if (!thread_running_.load())
    {
        return sep::SEPResult::INITIALIZATION_FAILED;
    }

    if (!obj || !handle_out)
    {
        return sep::SEPResult::INVALID_ARGUMENT;
    }

    // Validate configuration
    if (!isValidConfig(config))
    {
        return sep::SEPResult::INVALID_ARGUMENT;
    }

    {
        // Create new object state
        BlenderBridge::ObjectState state;
        state.object        = obj;
        state.config        = config;
        state.state         = PatternStateEnum::INITIALIZING;
        state.needs_update  = false;
        state.is_processing = false;

        // Initialize metrics
        state.metrics      = PatternMetrics();
        state.memory_usage = {0, 0, 0};
        state.stats        = {0.0f, 0, 0, 0.0f};

        // Allocate initial memory
        sep::SEPResult result = allocatePatternMemory(state);
        if (result != sep::SEPResult::SUCCESS)
        {
            notifyError(result, "Failed to allocate pattern memory");
            return result;
        }

        // Generate object handle
        ObjectHandle handle = next_handle_++;

        // Store object state
        {
            std::lock_guard<std::mutex> lock(objects_mutex_);
            objects_[handle] = std::move(state);
        }

        // Update output handle
        *handle_out = handle;

        // Notify state change
        notifyStateChange(handle, PatternStateEnum::UNINITIALIZED, PatternStateEnum::INITIALIZING);

        // Update resource stats
        updateResourceStats();

        return sep::SEPResult::SUCCESS;
    }
}

void BlenderBridge::addObserver(std::shared_ptr<PatternObserver> observer)
{
    if (!observer)
        return;

    std::lock_guard<std::mutex> lock(observers_mutex_);
    observers_.push_back(observer);
}

void BlenderBridge::removeObserver(std::shared_ptr<PatternObserver> observer)
{
    if (!observer)
        return;

    std::lock_guard<std::mutex> lock(observers_mutex_);
    observers_.erase(std::find(observers_.begin(), observers_.end(), observer));
}

void BlenderBridge::notifyObservers(ObjectHandle handle, const PatternMetrics& metrics)
{
    std::lock_guard<std::mutex> lock(observers_mutex_);
    for (const auto& observer : observers_)
    {
        observer->onPatternUpdate(handle, metrics);
    }
}

void BlenderBridge::notifyStateChange(ObjectHandle handle, PatternStateEnum old_state, PatternStateEnum new_state)
{
    std::lock_guard<std::mutex> lock(observers_mutex_);
    for (const auto& observer : observers_)
    {
        observer->onStateChange(handle, old_state, new_state);
    }
}

void BlenderBridge::notifyError(sep::SEPResult error, const char* message)
{
    std::lock_guard<std::mutex> lock(observers_mutex_);
    for (const auto& observer : observers_)
    {
        observer->onError(error, message);
    }
}

void BlenderBridge::notifyResourceWarning(ResourceType type, float utilization)
{
    std::lock_guard<std::mutex> lock(observers_mutex_);
    for (const auto& observer : observers_)
    {
        observer->onResourceWarning(type, utilization);
    }
}

bool BlenderBridge::isValidHandle(ObjectHandle handle) const
{
    std::lock_guard<std::mutex> lock(objects_mutex_);
    return objects_.find(handle) != objects_.end();
}

BlenderBridge::ObjectState* BlenderBridge::getObjectState(sep::pattern::ObjectHandle handle)
{
    std::lock_guard<std::mutex> lock(objects_mutex_);
    auto                        it = objects_.find(handle);
    return it != objects_.end() ? &it->second : nullptr;
}

void BlenderBridge::cleanupObject(sep::pattern::ObjectHandle handle)
{
    std::lock_guard<std::mutex> lock(objects_mutex_);
    auto                        it = objects_.find(handle);
    if (it != objects_.end())
    {
        freePatternMemory(it->second);
        objects_.erase(it);
    }
}

sep::SEPResult BlenderBridge::allocatePatternMemory(BlenderBridge::ObjectState& state)
{
    std::size_t bytes  = state.config.max_patterns * sizeof(sep::pattern::PatternData);
    auto&       mgr    = sep::memory::MemoryTierManager::getInstance();
    state.memory_block = mgr.allocate(bytes, sep::memory::TierType::DEVICE);
    if (!state.memory_block)
    {
        return sep::SEPResult::ALLOCATION_FAILED;
    }
    state.patterns.resize(state.config.max_patterns);
    state.memory_usage.host_memory = bytes;
    return sep::SEPResult::SUCCESS;
}

sep::SEPResult BlenderBridge::freePatternMemory(BlenderBridge::ObjectState& state)
{
    auto& mgr = sep::memory::MemoryTierManager::getInstance(); 
    if (state.memory_block) 
    {
        mgr.deallocate(state.memory_block);
        state.memory_block = nullptr;
    }
    state.patterns.clear();
    state.memory_usage.host_memory = 0;
    return sep::SEPResult::SUCCESS;
}

sep::SEPResult BlenderBridge::syncMemory(::sep::memory::MemoryTierEnum tier, bool force)
{
    if (!thread_running_.load())
    {
        return sep::SEPResult::INITIALIZATION_FAILED;
    }

    auto& manager = sep::memory::MemoryTierManager::getInstance();

    switch (tier)
    {
        case ::sep::memory::MemoryTierEnum::STM:
            manager.defragmentTier(sep::memory::TierType::HOST);
            break;
        case ::sep::memory::MemoryTierEnum::MTM:
            manager.defragmentTier(sep::memory::TierType::DEVICE);
            if (force)
            {
                manager.defragmentTier(sep::memory::TierType::HOST);
            }
            break;
        case ::sep::memory::MemoryTierEnum::LTM:
            manager.defragmentTier(sep::memory::TierType::UNIFIED);
            break;
        default:
            return sep::SEPResult::INVALID_STATE;
    }

    // Note: synchronize method not available in current MemoryTierManager
    // interface manager.synchronize();
    return sep::SEPResult::SUCCESS;
}

void BlenderBridge::startProcessingThread()
{
    if (thread_running_.load()) {
        return;
    }

    if (!gpu_context_) {
        return;
    }

    // Launch background thread for pattern processing
    thread_running_.store(true);
    processing_thread_ = std::thread(&BlenderBridge::processingThreadMain, this);
}

void BlenderBridge::stopProcessingThread()
{
    if (!thread_running_.load()) {
        return;
    }

    thread_running_.store(false);
    if (processing_thread_.joinable()) {
        processing_thread_.join();
    }
}

sep::SEPResult BlenderBridge::processPatterns()
{
    if (!thread_running_.load()) {
        return sep::SEPResult::INITIALIZATION_FAILED;
    }

    // Snapshot object list to avoid holding the lock while processing
    std::vector<std::pair<ObjectHandle, BlenderBridge::ObjectState*>> to_process;
    {
        std::lock_guard<std::mutex> lock(objects_mutex_);
        for (auto& [h, s] : objects_) {
            to_process.emplace_back(h, &s);
        }
    }

    for (auto& p : to_process) {
        processObjectPatterns(p.first, *p.second);
    }

    updateResourceStats();
    return sep::SEPResult::SUCCESS;
}

sep::SEPResult BlenderBridge::updateObject(ObjectHandle handle, const PatternMetrics& metrics)
{
    if (!thread_running_.load()) {
 return sep::SEPResult::INITIALIZATION_FAILED; 
    }
    
    if (!isValidHandle(handle)) {
        return sep::SEPResult::INVALID_ARGUMENT;
    }
    
    auto* state = getObjectState(handle);
    if (!state) {
        return sep::SEPResult::INVALID_ARGUMENT;
    }
    
    // Update object with new metrics
    state->metrics = metrics;
    
    // Notify observers
    notifyObservers(handle, metrics);
    
    return sep::SEPResult::SUCCESS;
}

void BlenderBridge::updateResourceStats()
{
    if (!thread_running_.load()) {
        return;
    }

    auto& manager = sep::memory::MemoryTierManager::getInstance();

    float stm_util = manager.getTierUtilization(
        static_cast<sep::memory::TierType>(::sep::memory::MemoryTierEnum::STM));
    float mtm_util = manager.getTierUtilization(
        static_cast<sep::memory::TierType>(::sep::memory::MemoryTierEnum::MTM));
    float ltm_util = manager.getTierUtilization(
        static_cast<sep::memory::TierType>(::sep::memory::MemoryTierEnum::LTM));

    if (stm_util > 0.9f)
        notifyResourceWarning(ResourceType::HOST_MEMORY, stm_util);
    if (mtm_util > 0.9f)
        notifyResourceWarning(ResourceType::GPU_MEMORY, mtm_util);
    if (ltm_util > 0.9f)
        notifyResourceWarning(ResourceType::STORAGE_RESOURCES, ltm_util);
}

void BlenderBridge::processingThreadMain()
{
    while (thread_running_.load()) {
        processPatterns();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

sep::SEPResult BlenderBridge::processObjectPatterns(sep::pattern::ObjectHandle handle, BlenderBridge::ObjectState& state)
{
    if (!state.object) {
        return sep::SEPResult::INVALID_ARGUMENT;
    }

    state.is_processing = true;

    // Run pattern evolution using the processor
    for (auto& pat : state.patterns) {
        pattern_processor_->addPattern(pat);
    }

    const auto& results = pattern_processor_->process();
    state.patterns.assign(results.begin(), results.end());
    // pattern count tracked via metrics

    updatePatternMetrics(state);
    validatePatternCoherence(state);

    state.stats.update_count++;
    notifyObservers(handle, state.metrics);

    state.is_processing = false;
    return sep::SEPResult::SUCCESS;
}

sep::SEPResult BlenderBridge::updatePatternMetrics(BlenderBridge::ObjectState& state)
{
    const auto pattern_count = state.patterns.size();
    state.metrics.active_patterns = pattern_count;

    float coherence_sum = 0.0f;
    float stability_sum = 0.0f;
    float max_entropy   = 0.0f;
    std::size_t mutation_total = 0;

    for (const auto& pat : state.patterns) {
        coherence_sum += pat.coherence;
        stability_sum += pat.stability;
        max_entropy    = std::max(max_entropy, pat.entropy);
        mutation_total += pat.mutation_count;
    }

    state.metrics.avg_coherence = pattern_count ? coherence_sum / pattern_count : 0.0f;
    state.metrics.peak_entropy  = max_entropy;
    state.metrics.updates_processed = state.stats.update_count;
    state.metrics.evolution.mutations = mutation_total;
    state.metrics.evolution.stability = pattern_count ? stability_sum / pattern_count : 0.0f;
    state.metrics.performance.process_time = state.stats.processing_time;
    state.metrics.performance.gpu_utilization = 0.0f;

    return sep::SEPResult::SUCCESS;
}

sep::SEPResult BlenderBridge::validatePatternCoherence(const BlenderBridge::ObjectState& state)
{
    for (const auto& pat : state.patterns) {
        if (pat.coherence < PatternLimits::MIN_COHERENCE_VALUE ||
            pat.coherence > PatternLimits::MAX_COHERENCE) {
            return sep::SEPResult::INVALID_STATE;
        }
    }
    return sep::SEPResult::SUCCESS;
}

sep::SEPResult BlenderBridge::promotePatterns(ObjectHandle handle, ::sep::memory::MemoryTierEnum target_tier)
{
    (void)handle;
    (void)target_tier;
    return sep::SEPResult::SUCCESS;
}

sep::SEPResult BlenderBridge::syncPatternData(ObjectHandle handle, bool force)
{
    (void)handle;
    (void)force;
    return sep::SEPResult::SUCCESS;
}

sep::SEPResult BlenderBridge::checkResourceLimits()
{
    return sep::SEPResult::SUCCESS;
}

bool BlenderBridge::checkResourceThresholds()
{
    return true;
}

float BlenderBridge::calculateResourceUtilization(ResourceType type) const
{
    (void)type;
    return 0.0f;
}

}  // namespace pattern
}  // namespace sep
