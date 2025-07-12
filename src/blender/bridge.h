#pragma once

#include <atomic>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <thread>
#include <unordered_map>
#include <vector>

#include "blender/base_types.h"
#include "blender/blender_types.h"
#include "blender/config.h"
#include "blender/gpu_context.h"
#include "blender/pattern_common.h"
#include "blender/types.h"
#include "compat/shim.h"
#include "core/types.h"
#include "memory/memory_tier.hpp"
#include "quantum/processor.h"

namespace sep
{
    namespace pattern
    {

        struct PatternLimits
        {
            static constexpr size_t MAX_PATTERNS = 10000;
            static constexpr float MIN_COHERENCE_VALUE = 0.0f;
            static constexpr float MAX_COHERENCE = 1.0f;
        };

        struct PatternConfig
        {
            float update_threshold{0.0f};
            bool enable_mutations{false};
            int max_patterns{0};
            int batch_size{0};
        };
        // Forward declarations
        class PatternObserver;

        // Thread-safe bridge for coordinating Blender objects with the quantum processor.
        // Access to internal maps and observer lists is protected by mutexes.
        class BlenderBridge
        {
        public:
            BlenderBridge();
            BlenderBridge(const BlenderBridge&) = delete;
            BlenderBridge& operator=(const BlenderBridge&) = delete;
            BlenderBridge(BlenderBridge&&) noexcept = delete;
            BlenderBridge& operator=(BlenderBridge&&) noexcept = delete;
            virtual ~BlenderBridge();

            virtual sep::SEPResult init(
                ::sep::GPUContext* ctx);  // Use global namespace for GPUContext

            virtual sep::SEPResult registerObject(  // Changed to global SEPResult
                Object* obj, const sep::pattern::PatternConfig& config,
                sep::pattern::ObjectHandle* handle_out);

            virtual sep::SEPResult updateObject(  // Changed to global SEPResult
                sep::pattern::ObjectHandle handle, const sep::pattern::PatternMetrics& metrics);

            virtual sep::SEPResult processPatterns();  // Changed to global SEPResult

            struct ObjectState
            {
                Object* object = nullptr;
                sep::pattern::PatternConfig config{};
                sep::pattern::PatternMetrics metrics{};
                sep::pattern::PatternStateEnum state =
                    sep::pattern::PatternStateEnum::UNINITIALIZED;
                sep::pattern::PatternState pattern_state{};
                std::vector<sep::pattern::PatternData> patterns;
                sep::memory::MemoryBlock* memory_block = nullptr;
                bool needs_update = false;
                bool is_processing = false;
            };

            void addObserver(std::shared_ptr<PatternObserver> observer);
            void removeObserver(std::shared_ptr<PatternObserver> observer);
            static std::unique_ptr<BlenderBridge> create();

            void startProcessingThread();
            void stopProcessingThread();
            void processingThreadMain();
            void notifyObservers(sep::pattern::ObjectHandle handle,
                                 const sep::pattern::PatternMetrics& metrics);
            void notifyStateChange(sep::pattern::ObjectHandle handle,
                                   sep::pattern::PatternStateEnum old_state,
                                   sep::pattern::PatternStateEnum new_state);
            void notifyError(sep::SEPResult error, const char* message);
            void notifyResourceWarning(sep::pattern::ResourceType type, float utilization);

            sep::SEPResult processObjectPatterns(sep::pattern::ObjectHandle handle,
                                                 ObjectState& state);
            sep::SEPResult updatePatternMetrics(ObjectState& state);
            sep::SEPResult validatePatternCoherence(const ObjectState& state);
            sep::SEPResult checkResourceLimits();
            sep::SEPResult allocatePatternMemory(ObjectState& state);
            sep::SEPResult freePatternMemory(ObjectState& state);
            sep::SEPResult promotePatterns(sep::pattern::ObjectHandle handle,
                                           ::sep::memory::MemoryTierEnum target_tier);
            sep::SEPResult syncPatternData(sep::pattern::ObjectHandle handle, bool force);
            sep::SEPResult syncMemory(::sep::memory::MemoryTierEnum tier, bool force);
            bool isInitialized() const { return initialized_; }
            bool isValidHandle(sep::pattern::ObjectHandle handle) const;
            ObjectState* getObjectState(sep::pattern::ObjectHandle handle);
            void cleanup();
            void cleanupObject(sep::pattern::ObjectHandle handle);

            void updateResourceStats();
            float calculateResourceUtilization(sep::pattern::ResourceType type) const;
            bool checkResourceThresholds();

            sep::GPUContext* gpu_context_;  // Changed to sep::GPUContext
            std::unique_ptr<sep::pattern::PatternProcessor>
                pattern_processor_;  // Base pattern processor from processor.h

            // Access to objects_ must be guarded by objects_mutex_
            std::unordered_map<sep::pattern::ObjectHandle, std::unique_ptr<ObjectState>> objects_;
            // observers_ is modified by multiple threads and protected by observers_mutex_
            std::vector<std::shared_ptr<sep::pattern::PatternObserver>> observers_;

            std::atomic<bool> initialized_{false};
            std::atomic<bool> processing_{false};
            std::atomic<sep::pattern::ObjectHandle> next_handle_{1};

            // Mutexes guarding access to shared maps and observer lists
            std::mutex objects_mutex_;
            // guards observers_ list
            mutable std::mutex observers_mutex_;
            std::mutex processing_mutex_;
            std::condition_variable processing_cv_;

            std::thread processing_thread_;
            std::atomic<bool> thread_running_{false};

            struct
            {
                std::atomic<size_t> total_patterns{0};
                std::atomic<size_t> active_patterns{0};
                std::atomic<uint64_t> updates_processed{0};
                std::atomic<float> avg_process_time{0.0f};
                std::atomic<size_t> memory_used{0};
                std::atomic<float> gpu_utilization{0.0f};
                std::atomic<size_t> pattern_mutations{0};
                std::atomic<float> avg_coherence{0.0f};
                std::atomic<float> peak_entropy{0.0f};
            } stats_;

            struct
            {
                float gpu_memory_threshold{0.9f};
                float host_memory_threshold{0.8f};
                float process_time_threshold{16.0f};
                size_t max_patterns_per_object{PatternLimits::MAX_PATTERNS};
                float min_coherence{PatternLimits::MIN_COHERENCE_VALUE};
            } thresholds_;
        };
    };
}  // namespace pattern
}  // namespace sep