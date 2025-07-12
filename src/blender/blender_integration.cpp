#include <unistd.h>

#include "blender/pattern_bridge.h"
#include "blender_pch.h"
#include "core/types.h"
#include "memory/memory_tier_manager.hpp"
#include "quantum/processor.h"

using ::sep::memory::MemoryTierEnum;

namespace sep
{
    namespace pattern
    {

        BlenderBridge::BlenderBridge() : gpu_context_(nullptr), thread_running_(false) {}

        BlenderBridge::~BlenderBridge() = default;

        std::unique_ptr<BlenderBridge> BlenderBridge::create()
        {
            return std::unique_ptr<BlenderBridge>(new (std::nothrow) BlenderBridge());
        }

        SEPResult BlenderBridge::init(GPUContext* ctx)
        {
            if (!ctx)
            {
                return SEPResult::INVALID_ARGUMENT;
            }

            if (thread_running_.load())
            {
                return SEPResult::ALREADY_EXISTS;
            }

            gpu_context_ = ctx;
            pattern_processor_ = std::make_unique<PatternProcessor>();
            if (!pattern_processor_)
            {
                return SEPResult::ALLOCATION_FAILED;
            }

            startProcessingThread();
            return SEPResult::SUCCESS;
        }

        SEPResult BlenderBridge::registerObject(Object* obj, const PatternConfig& config,
                                                ObjectHandle* handle_out)
        {
            if (!thread_running_.load())
            {
                return SEPResult::INITIALIZATION_FAILED;
            }
            if (!obj || !handle_out || config.max_patterns <= 0)
            {
                return SEPResult::INVALID_ARGUMENT;
            }

            auto state = std::make_unique<ObjectState>();
            state->object = obj;
            state->config = config;
            state->state = PatternStateEnum::INITIALIZING;

            SEPResult res = allocatePatternMemory(*state);
            if (res != SEPResult::SUCCESS)
            {
                return res;
            }

            ObjectHandle handle = next_handle_++;
            {
                std::lock_guard<std::mutex> lock(objects_mutex_);
                objects_.emplace(handle, std::move(state));
            }

            *handle_out = handle;
            notifyStateChange(handle, PatternStateEnum::UNINITIALIZED,
                              PatternStateEnum::INITIALIZING);
            updateResourceStats();
            return SEPResult::SUCCESS;
        }

        void BlenderBridge::addObserver(std::shared_ptr<PatternObserver> observer)
        {
            if (!observer) return;
            std::lock_guard<std::mutex> lock(observers_mutex_);
            observers_.push_back(observer);
        }

        void BlenderBridge::removeObserver(std::shared_ptr<PatternObserver> observer)
        {
            if (!observer) return;
            std::lock_guard<std::mutex> lock(observers_mutex_);
            observers_.erase(std::remove(observers_.begin(), observers_.end(), observer),
                             observers_.end());
        }

        void BlenderBridge::notifyObservers(ObjectHandle handle, const PatternMetrics& metrics)
        {
            std::lock_guard<std::mutex> lock(observers_mutex_);
            for (const auto& obs : observers_)
            {
                obs->onPatternUpdate(handle, metrics);
            }
        }

        void BlenderBridge::notifyStateChange(ObjectHandle handle, PatternStateEnum old_state,
                                              PatternStateEnum new_state)
        {
            std::lock_guard<std::mutex> lock(observers_mutex_);
            for (const auto& obs : observers_)
            {
                obs->onStateChange(handle, old_state, new_state);
            }
        }

        void BlenderBridge::notifyError(SEPResult error, const char* message)
        {
            std::lock_guard<std::mutex> lock(observers_mutex_);
            for (const auto& obs : observers_)
            {
                obs->onError(error, message);
            }
        }

        void BlenderBridge::notifyResourceWarning(ResourceType type, float utilization)
        {
            std::lock_guard<std::mutex> lock(observers_mutex_);
            for (const auto& obs : observers_)
            {
                obs->onResourceWarning(type, utilization);
            }
        }

        bool BlenderBridge::isValidHandle(ObjectHandle handle) const
        {
            std::lock_guard<std::mutex> lock(objects_mutex_);
            return objects_.find(handle) != objects_.end();
        }

        BlenderBridge::ObjectState* BlenderBridge::getObjectState(ObjectHandle handle)
        {
            std::lock_guard<std::mutex> lock(objects_mutex_);
            auto it = objects_.find(handle);
            return it != objects_.end() ? it->second.get() : nullptr;
        }

        void BlenderBridge::cleanupObject(ObjectHandle handle)
        {
            std::unique_ptr<ObjectState> state;
            {
                std::lock_guard<std::mutex> lock(objects_mutex_);
                auto it = objects_.find(handle);
                if (it == objects_.end()) return;
                state = std::move(it->second);
                objects_.erase(it);
            }
            if (state)
            {
                freePatternMemory(*state);
            }
        }

        SEPResult BlenderBridge::allocatePatternMemory(ObjectState& state)
        {
            std::size_t bytes = state.config.max_patterns * sizeof(PatternData);
            auto& mgr = memory::MemoryTierManager::getInstance();
            state.memory_block = mgr.allocate(bytes, MemoryTierEnum::DEVICE);
            if (!state.memory_block)
            {
                return SEPResult::ALLOCATION_FAILED;
            }
            state.patterns.resize(state.config.max_patterns);
            state.memory_usage.host_memory = bytes;
            return SEPResult::SUCCESS;
        }

        SEPResult BlenderBridge::freePatternMemory(ObjectState& state)
        {
            auto& mgr = memory::MemoryTierManager::getInstance();
            if (state.memory_block)
            {
                mgr.deallocate(state.memory_block);
                state.memory_block = nullptr;
            }
            state.patterns.clear();
            state.memory_usage.host_memory = 0;
            return SEPResult::SUCCESS;
        }

        SEPResult BlenderBridge::syncMemory(MemoryTierEnum tier, bool force)
        {
            if (!thread_running_.load())
            {
                return SEPResult::INITIALIZATION_FAILED;
            }
            auto& mgr = memory::MemoryTierManager::getInstance();
            mgr.defragmentTier(tier);
            if (force)
            {
                mgr.defragmentTier(MemoryTierEnum::HOST);
            }
            return SEPResult::SUCCESS;
        }

        void BlenderBridge::startProcessingThread()
        {
            if (thread_running_.load()) return;
            thread_running_.store(true);
            processing_thread_ = std::thread(&BlenderBridge::processingThreadMain, this);
        }

        void BlenderBridge::stopProcessingThread()
        {
            if (!thread_running_.load()) return;
            thread_running_.store(false);
            if (processing_thread_.joinable())
            {
                processing_thread_.join();
            }
        }

        SEPResult BlenderBridge::processPatterns()
        {
            if (!thread_running_.load())
            {
                return SEPResult::INITIALIZATION_FAILED;
            }

            std::vector<std::pair<ObjectHandle, ObjectState*>> local_objects;
            {
                std::lock_guard<std::mutex> lock(objects_mutex_);
                for (auto& kv : objects_)
                {
                    local_objects.emplace_back(kv.first, kv.second.get());
                }
            }

            for (auto& p : local_objects)
            {
                processObjectPatterns(p.first, *p.second);
            }
            updateResourceStats();
            return SEPResult::SUCCESS;
        }

        SEPResult BlenderBridge::updateObject(ObjectHandle handle, const PatternMetrics& metrics)
        {
            if (!isValidHandle(handle))
            {
                return SEPResult::INVALID_ARGUMENT;
            }
            auto* state = getObjectState(handle);
            if (!state) return SEPResult::INVALID_ARGUMENT;
            state->metrics = metrics;
            notifyObservers(handle, metrics);
            return SEPResult::SUCCESS;
        }

        void BlenderBridge::updateResourceStats()
        {
            auto& mgr = memory::MemoryTierManager::getInstance();
            float host = mgr.getTierUtilization(MemoryTierEnum::HOST);
            float dev = mgr.getTierUtilization(MemoryTierEnum::DEVICE);
            if (host > 0.9f) notifyResourceWarning(ResourceType::HOST_MEMORY, host);
            if (dev > 0.9f) notifyResourceWarning(ResourceType::GPU_MEMORY, dev);
        }

        void BlenderBridge::processingThreadMain()
        {
            while (thread_running_.load())
            {
                processPatterns();
                usleep(10000);  // sleep 10ms
            }
        }

        SEPResult BlenderBridge::processObjectPatterns(ObjectHandle handle, ObjectState& state)
        {
            (void)handle;
            for (auto& pat : state.patterns)
            {
                pattern_processor_->addPattern(pat);
            }
            auto results = pattern_processor_->process();
            state.patterns.assign(results.begin(), results.end());
            updatePatternMetrics(state);
            validatePatternCoherence(state);
            return SEPResult::SUCCESS;
        }

        SEPResult BlenderBridge::updatePatternMetrics(ObjectState& state)
        {
            const size_t count = state.patterns.size();
            state.metrics.active_patterns = count;
            float coh = 0.0f;
            float ent = 0.0f;
            for (const auto& p : state.patterns)
            {
                coh += p.coherence;
                ent = std::max(ent, p.entropy);
            }
            state.metrics.avg_coherence = count ? coh / count : 0.0f;
            state.metrics.peak_entropy = ent;
            return SEPResult::SUCCESS;
        }

        SEPResult BlenderBridge::validatePatternCoherence(const ObjectState& state)
        {
            for (const auto& p : state.patterns)
            {
                if (p.coherence < PatternLimits::MIN_COHERENCE_VALUE ||
                    p.coherence > PatternLimits::MAX_COHERENCE)
                {
                    return SEPResult::INVALID_STATE;
                }
            }
            return SEPResult::SUCCESS;
        }

        SEPResult BlenderBridge::promotePatterns(ObjectHandle, MemoryTierEnum)
        {
            return SEPResult::SUCCESS;
        }

        SEPResult BlenderBridge::syncPatternData(ObjectHandle, bool) { return SEPResult::SUCCESS; }

        SEPResult BlenderBridge::checkResourceLimits() { return SEPResult::SUCCESS; }

        bool BlenderBridge::checkResourceThresholds() { return true; }

        float BlenderBridge::calculateResourceUtilization(ResourceType) const { return 0.0f; }

    }  // namespace pattern
}  // namespace sep
