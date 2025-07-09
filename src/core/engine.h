#ifndef SEP_CORE_ENGINE_H
#define SEP_CORE_ENGINE_H

#include <cstdint>
#include <memory>
#include <vector>

#include "blender/types.h"  // SEPBlenderBridge definition
#include "compat/shim.h"
#include "compat/types.h"  // for QSHResult
#include "core/common.h"
#include "core/types.h"
#include "quantum/qbsa.h"

namespace sep
{
    namespace cuda
    {
        class Stream;
        template <typename T>
        class DeviceMemory;
        using StreamPtr = std::shared_ptr<Stream>;
        struct QSHResult;
    }  // namespace cuda
    namespace audio
    {
        class AudioCapture;
    }  // namespace audio
    namespace pattern
    {
        class BlenderBridge;
    }  // namespace pattern

    namespace core
    {

        /**
         * @brief Main quantum processing engine
         */
        class Engine
        {
        public:
            Engine() noexcept(false);
            ~Engine();

            // Delete copy operations
            Engine(const Engine &) = delete;
            Engine &operator=(const Engine &) = delete;

            // Allow move operations
            Engine(Engine &&) noexcept = default;
            Engine &operator=(Engine &&) noexcept = default;

            // Explicit initialization and lifecycle management
            bool init(const ::sep::config::APIConfig &config);
            void run();
            void shutdown();

            void generate_probes(const std::vector<::sep::PinState> &inputs,
                                 std::vector<std::uint32_t> &indices,
                                 std::vector<std::uint32_t> &expectations, std::uint64_t tick);

            void process_batch(const std::vector<::sep::PinState> &inputs, std::uint64_t tick,
                               ::sep::quantum::QBSAResult &qbsa_result,
                               ::sep::cuda::QSHResult &qsh_result);

            // DAG accessors
            struct StateNode
            {
                std::uint64_t tick{0};
                float coherence{0.0f};
                bool rupture{false};
                std::vector<std::size_t> parents;
            };

            const std::vector<StateNode> &getStateHistory() const noexcept;

            std::vector<float> getCoherenceHistory() const;

        private:
            static constexpr size_t DEFAULT_SIZE = 1024;
            static constexpr size_t PAIRS_PER_CHUNK = 32;  // WARP_SIZE

            struct Impl;
            std::unique_ptr<Impl> impl_;

            // Managed components
#ifdef SEP_HAS_AUDIO
            std::unique_ptr<::sep::audio::AudioCapture> audio_capture_;
#endif
#ifdef SEP_HAS_BLENDER
            std::unique_ptr<SEPBlenderBridge> blender_bridge_;
#endif
        };
}  // namespace core
}  // namespace sep

#endif  // SEP_CORE_ENGINE_H

