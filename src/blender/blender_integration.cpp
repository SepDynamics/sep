#include "blender/pattern_bridge.h"
#include <thread>
#include <chrono>

#include <algorithm>
#include <memory>
#include <new>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "quantum/processor.h"  // Ensure PatternProcessor is complete

#include "blender/pattern_observer.h"
#include "quantum/types.h"
#include "memory/types.h"
#include "core/common.h"  // For sep::SEPResult

#include "memory/memory_tier_manager.hpp"
#include "compat/component_bridge.h" // For createBlenderBridge


namespace sep {
namespace pattern {
// Use the core SEPResult enum instead of the pattern-specific one
// Constructor implementation
BlenderBridge::BlenderBridge()
    : m_processing_thread_active(false)
    , m_gpu_context(nullptr)
{
    // Initialize the bridge
}

BlenderBridge::~BlenderBridge() = default;


std::shared_ptr<BlenderBridge> BlenderBridge::create()
{
    return std::shared_ptr<BlenderBridge>(new (std::nothrow) BlenderBridge());
}


sep::SEPResult BlenderBridge::init(sep::GPUContext* ctx)
{
    if (!ctx)
    {
        return sep::SEPResult::INVALID_ARGUMENT;
    }

    if (m_processing_thread_active.load())
    {
        return sep::SEPResult::ALREADY_EXISTS;
    }

    // Store GPU context
    m_gpu_context = ctx;

    // Initialize pattern processor
    m_pattern_processor.reset(new (std::nothrow) sep::pattern::PatternProcessor());
    if (!m_pattern_processor)
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
    if (!m_processing_thread_active.load())
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
        ObjectState state;
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

    std::lock_guard<std::mutex> lock(m_observer_mutex);
    m_observers.push_back(observer);
}

void BlenderBridge::removeObserver(std::shared_ptr<PatternObserver> observer)
{
    if (!observer)
        return;

    std::lock_guard<std::mutex> lock(m_observer_mutex);
    m_observers.erase(std::find(m_observers.begin(), m_observers.end(), observer));
}

void BlenderBridge::notifyObservers(ObjectHandle handle, const PatternMetrics& metrics)
{
    std::lock_guard<std::mutex> lock(m_observer_mutex);
    for (const auto& observer : m_observers)
    {
        observer->onPatternUpdate(handle, metrics);
    }
}

void BlenderBridge::notifyStateChange(ObjectHandle handle, PatternStateEnum old_state, PatternStateEnum new_state)
{
    std::lock_guard<std::mutex> lock(m_observer_mutex);
    for (const auto& observer : m_observers)
    {
        observer->onStateChange(handle, old_state, new_state);
    }
}

void BlenderBridge::notifyError(sep::SEPResult error, const char* message)
{
    std::lock_guard<std::mutex> lock(m_observer_mutex);
    for (const auto& observer : m_observers)
    {
        // Convert from sep::SEPResult to sep::pattern::PatternResult
        sep::pattern::PatternResult pattern_error;
        
        // Map the most common error codes
        switch (error) {
            case sep::SEPResult::SUCCESS:
                pattern_error = sep::pattern::PatternResult::SUCCESS;
                break;
            case sep::SEPResult::INVALID_ARGUMENT:
                pattern_error = sep::pattern::PatternResult::INVALID_ARGUMENT;
                break;
            case sep::SEPResult::ALLOCATION_FAILED:
                pattern_error = sep::pattern::PatternResult::ALLOCATION_FAILED;
                break;
            case sep::SEPResult::PROCESSING_ERROR:
                pattern_error = sep::pattern::PatternResult::PROCESSING_ERROR;
                break;
            default:
                // Default to PROCESSING_ERROR for any unmapped error
                pattern_error = sep::pattern::PatternResult::PROCESSING_ERROR;
                break;
        }
        
        observer->onError(pattern_error, message);
    }
}

void BlenderBridge::notifyResourceWarning(ResourceType type, float utilization)
{
    std::lock_guard<std::mutex> lock(m_observer_mutex);
    for (const auto& observer : m_observers)
    {
        observer->onResourceWarning(type, utilization);
    }
}

bool BlenderBridge::isValidHandle(ObjectHandle handle) const
{
    std::lock_guard<std::mutex> lock(objects_mutex_);
    return objects_.find(handle) != objects_.end();
}

ObjectState* BlenderBridge::getObjectStatePtr(sep::pattern::ObjectHandle handle)
{
    std::lock_guard<std::mutex> lock(objects_mutex_);
    auto                        it = objects_.find(handle);
    return it != objects_.end() ? &it->second : nullptr;
}

sep::SEPResult BlenderBridge::cleanupObject(sep::pattern::ObjectHandle handle)
{
    std::lock_guard<std::mutex> lock(objects_mutex_);
    auto                        it = objects_.find(handle);
    if (it != objects_.end())
    {
        freePatternMemory(it->second);
        objects_.erase(it);
    }
    return sep::SEPResult::SUCCESS;
}

sep::SEPResult BlenderBridge::allocatePatternMemory(ObjectState& state)
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

sep::SEPResult BlenderBridge::freePatternMemory(ObjectState& state)
{
    auto& mgr = sep::memory::MemoryTierManager::getInstance(); // Fix: Add missing variable definition
    if (state.memory_block) // Fix: Add if condition
    {
        mgr.deallocate(state.memory_block);
        state.memory_block = nullptr;
    }
    state.patterns.clear();
    state.memory_usage.host_memory = 0;
    return sep::SEPResult::SUCCESS;
}

sep::SEPResult BlenderBridge::syncMemory(MemoryTierEnum tier, bool force)
{
    if (!m_processing_thread_active.load())
    {
        return sep::SEPResult::INITIALIZATION_FAILED;
    }

    auto& manager = sep::memory::MemoryTierManager::getInstance();

    switch (tier)
    {
        case MemoryTierEnum::STM:
            manager.defragmentTier(sep::memory::TierType::HOST);
            break;
        case MemoryTierEnum::MTM:
            manager.defragmentTier(sep::memory::TierType::DEVICE);
            if (force)
            {
                manager.defragmentTier(sep::memory::TierType::HOST);
            }
            break;
        case MemoryTierEnum::LTM:
            manager.defragmentTier(sep::memory::TierType::UNIFIED);
            break;
        default:
            return sep::SEPResult::INVALID_STATE;
    }

    // Note: synchronize method not available in current MemoryTierManager
    // interface manager.synchronize();
    return sep::SEPResult::SUCCESS;
}

sep::SEPResult BlenderBridge::startProcessingThread()
{
    if (m_processing_thread_active.load()) {
        return sep::SEPResult::ALREADY_EXISTS;
    }
    
    if (!m_gpu_context) {
        return sep::SEPResult::INITIALIZATION_FAILED;
    }

    // Launch background thread for pattern processing
    m_processing_thread_active.store(true);
    m_processing_thread.reset(
        new std::thread(&BlenderBridge::processingThreadMain, this));

    return sep::SEPResult::SUCCESS;
}

sep::SEPResult BlenderBridge::stopProcessingThread()
{
    if (!m_processing_thread_active.load()) {
        return sep::SEPResult::INITIALIZATION_FAILED;
    }
    
    m_processing_thread_active.store(false);
    if (m_processing_thread && m_processing_thread->joinable()) {
        m_processing_thread->join();
    }
    return sep::SEPResult::SUCCESS;
}

sep::SEPResult BlenderBridge::processPatterns()
{
    if (!m_processing_thread_active.load()) {
        return sep::SEPResult::INITIALIZATION_FAILED;
    }

    // Snapshot object list to avoid holding the lock while processing
    std::vector<std::pair<ObjectHandle, ObjectState*>> to_process;
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
    if (!m_processing_thread_active.load()) {
        return sep::SEPResult::INITIALIZATION_FAILED;
    }
    
    if (!isValidHandle(handle)) {
        return sep::SEPResult::INVALID_ARGUMENT;
    }
    
    auto* state = getObjectStatePtr(handle);
    if (!state) {
        return sep::SEPResult::INVALID_ARGUMENT;
    }
    
    // Update object with new metrics
    state->metrics = metrics;
    
    // Notify observers
    notifyObservers(handle, metrics);
    
    return sep::SEPResult::SUCCESS;
}

sep::SEPResult BlenderBridge::updateResourceStats()
{
    if (!m_processing_thread_active.load()) {
        return sep::SEPResult::INITIALIZATION_FAILED;
    }

    auto& manager = sep::memory::MemoryTierManager::getInstance();

    float stm_util = manager.getTierUtilization(
        static_cast<sep::memory::TierType>(sep::MemoryTierEnum::STM));
    float mtm_util = manager.getTierUtilization(
        static_cast<sep::memory::TierType>(sep::MemoryTierEnum::MTM));
    float ltm_util = manager.getTierUtilization(
        static_cast<sep::memory::TierType>(sep::MemoryTierEnum::LTM));

    if (stm_util > 0.9f)
        notifyResourceWarning(ResourceType::HOST_MEMORY, stm_util);
    if (mtm_util > 0.9f)
        notifyResourceWarning(ResourceType::GPU_MEMORY, mtm_util);
    if (ltm_util > 0.9f)
        notifyResourceWarning(ResourceType::STORAGE_RESOURCES, ltm_util);

    return sep::SEPResult::SUCCESS;
}

void BlenderBridge::processingThreadMain()
{
    while (m_processing_thread_active.load()) {
        processPatterns();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

sep::SEPResult BlenderBridge::processObjectPatterns(sep::pattern::ObjectHandle handle, ObjectState& state)
{
    if (!state.object) {
        return sep::SEPResult::INVALID_ARGUMENT;
    }

    state.is_processing = true;

    // Run pattern evolution using the processor
    for (auto& pat : state.patterns) {
        m_pattern_processor->addPattern(pat);
    }

    const auto& results = m_pattern_processor->process();
    state.patterns.assign(results.begin(), results.end());
    state.stats.pattern_count = static_cast<uint32_t>(state.patterns.size());

    updatePatternMetrics(state);
    validatePatternCoherence(state);

    state.stats.update_count++;
    notifyObservers(handle, state.metrics);

    state.is_processing = false;
    return sep::SEPResult::SUCCESS;
}

sep::SEPResult BlenderBridge::updatePatternMetrics(ObjectState& state)
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

sep::SEPResult BlenderBridge::validatePatternCoherence(const ObjectState& state)
{
    for (const auto& pat : state.patterns) {
        if (pat.coherence < PatternLimits::MIN_COHERENCE_VALUE ||
            pat.coherence > PatternLimits::MAX_COHERENCE) {
            return sep::SEPResult::INVALID_STATE;
        }
    }
    return sep::SEPResult::SUCCESS;
}

sep::SEPResult BlenderBridge::promotePatterns(ObjectHandle handle, MemoryTierEnum target_tier)
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

sep::SEPResult BlenderBridge::checkResourceThresholds()
{
    return sep::SEPResult::SUCCESS;
}

float BlenderBridge::calculateResourceUtilization(ResourceType type)
{
    (void)type;
    return 0.0f;
}

}  // namespace pattern
}  // namespace sep
