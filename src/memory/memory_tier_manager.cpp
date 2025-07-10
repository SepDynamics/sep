#include "memory/memory_tier_manager.hpp"

#include "core/common.h"
#include "core/types.h"
#include "memory/memory_tier.hpp"
#include "memory/types.h"
#include "quantum/data.hpp"
#include "quantum/pattern.h"
#include "quantum/types.h"

namespace sep {
namespace pattern {
    struct PatternData;
}
namespace quantum {
    struct Pattern;
}
}

namespace sep
{
    namespace config
    {
        class ConfigManager;
    }
}

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <memory>
#include <mutex>
#include <new>
#include <string>

namespace sep
{
    namespace config
    {
        class ConfigManager;
    }  // namespace config
    namespace memory
    {

        // Forward declarations
        class MemoryTier;
        struct MemoryBlock;

        // Using declarations
        using sep::memory::MemoryTierEnum;

        // Static member initializations
        std::unique_ptr<MemoryTierManager> MemoryTierManager::instance_;
        std::once_flag MemoryTierManager::once_flag_;

        // Singleton getter
        MemoryTierManager &MemoryTierManager::getInstance()
        {
            std::call_once(once_flag_, []() {
                instance_ = std::make_unique<MemoryTierManager>();
                instance_->hooks_ = nullptr;
            });
            return *instance_;
        }

        // Constructor
        MemoryTierManager::MemoryTierManager()
            : stm_(std::make_unique<MemoryTier>(MemoryTierEnum::STM, 1 << 20, 0.7f, 5)),
              mtm_(std::make_unique<MemoryTier>(MemoryTierEnum::MTM, 4 << 20, 0.8f, 50)),
              ltm_(std::make_unique<MemoryTier>(MemoryTierEnum::LTM, 16 << 20, 0.9f, 100))
        {
            init(Config{});
        }

        MemoryTierManager::MemoryTierManager(const Config &cfg)
            : stm_(std::make_unique<MemoryTier>(MemoryTierEnum::STM, cfg.stm_size, cfg.promote_stm_to_mtm, cfg.stm_to_mtm_min_gen)),
              mtm_(std::make_unique<MemoryTier>(MemoryTierEnum::MTM, cfg.mtm_size, cfg.promote_mtm_to_ltm, cfg.mtm_to_ltm_min_gen)),
              ltm_(std::make_unique<MemoryTier>(MemoryTierEnum::LTM, cfg.ltm_size, 1.0f, 100))
        {
            init(cfg);
        }

        MemoryTierManager::~MemoryTierManager() = default;

        void MemoryTierManager::init(const Config &cfg)
        {
            config_ = cfg;
            rebuildLookup();
        }

        void MemoryTierManager::shutdown()
        {
            // Clean up
            lookup_map_.clear();
        }

        MemoryBlock *MemoryTierManager::allocate(std::size_t size, MemoryTierEnum tier)
        {
            MemoryTier *t = getTier(tier);
            if (!t) return nullptr;
            MemoryBlock *block = t->allocate(size);
            if (block)
            {
                lookup_map_[block->ptr] = block;
            }
            return block;
        }

        void MemoryTierManager::deallocate(MemoryBlock *block)
        {
            if (!block) return;
            MemoryTier *tier = getTier(block->tier);
            if (!tier) return;
            void *ptr = block->ptr;
            tier->deallocate(block);
            lookup_map_.erase(ptr);
        }

        MemoryBlock *MemoryTierManager::findBlockByPtr(void *ptr)
        {
            auto it = lookup_map_.find(ptr);
            if (it != lookup_map_.end())
            {
                return it->second;
            }
            auto it2 = legacy_lookup_map_.find(ptr);
            if (it2 != legacy_lookup_map_.end())
            {
                return it2->second;
            }
            return nullptr;
        }

        MemoryTier *MemoryTierManager::getTier(MemoryTierEnum tier)
        {
            switch (tier)
            {
                case MemoryTierEnum::STM:
                    return stm_.get();
                case MemoryTierEnum::MTM:
                    return mtm_.get();
                case MemoryTierEnum::LTM:
                    return ltm_.get();
                default:
                    return nullptr;
            }
        }

        float MemoryTierManager::getTierFragmentation(MemoryTierEnum tier) const
        {
            switch (tier)
            {
                case MemoryTierEnum::STM:
                case MemoryTierEnum::MTM:
                case MemoryTierEnum::LTM:
                default:
                    return 0.0f;
            }
        }

        float MemoryTierManager::getTotalUtilization() const
        {
            std::size_t total_size = stm_->getSize() + mtm_->getSize() + ltm_->getSize();
            std::size_t total_used =
                stm_->getUsedSpace() + mtm_->getUsedSpace() + ltm_->getUsedSpace();

            return total_size > 0 ? static_cast<float>(total_used) / static_cast<float>(total_size)
                                  : 0.0f;
        }

        float MemoryTierManager::getTotalFragmentation() const
        {
            return (stm_->calculateFragmentation() + mtm_->calculateFragmentation() +
                    ltm_->calculateFragmentation()) /
                   3.0f;
        }

        std::size_t MemoryTierManager::getTotalAllocated() const
        {
            return stm_->getUsedSpace() + mtm_->getUsedSpace() + ltm_->getUsedSpace();
        }

        MemoryTier &MemoryTierManager::getSTM() { return *stm_; }
        MemoryTier &MemoryTierManager::getMTM() { return *mtm_; }
        MemoryTier &MemoryTierManager::getLTM() { return *ltm_; }

        void MemoryTierManager::defragmentTier(MemoryTierEnum tier)
        {
            MemoryTier *t = getTier(tier);
            if (t)
            {
                t->defragment();
            }
        }

        void MemoryTierManager::optimizeTiers()
        {
            auto process_tier = [](MemoryTier *tier) {
                if (!tier) return;
                if (tier->calculateFragmentation() > 0.3f)
                {
                    tier->defragment();
                }
            };

            process_tier(stm_.get());
            process_tier(mtm_.get());
            process_tier(ltm_.get());
        }

        void MemoryTierManager::rebuildLookup()
        {
            // Clear the old lookup map but keep the old pointers in the legacy map
            // to handle transition periods.
            legacy_lookup_map_ = std::move(lookup_map_);
            lookup_map_.clear();

            auto process_tier = [this](MemoryTier *tier) {
                if (!tier) return;
                auto &blocks = tier->getBlocks();
                for (const auto &blk : blocks)
                {
                    if (blk.allocated)
                    {
                        lookup_map_[blk.ptr] = const_cast<MemoryBlock *>(&blk);
                    }
                }
            };

            process_tier(stm_.get());
            process_tier(mtm_.get());
            process_tier(ltm_.get());
        }

        void MemoryTierManager::optimizeBlocks()
        {
            auto process_tier = [this](MemoryTier *tier) {
                if (!tier) return;
                auto &blocks = const_cast<std::deque<MemoryBlock> &>(tier->getBlocks());
                for (auto &blk : blocks)
                {
                    if (blk.allocated)
                    {
                        updateBlockMetrics(&blk, blk.coherence, blk.stability, blk.generation,
                                           blk.weight);
                    }
                }
            };

            process_tier(stm_.get());
            process_tier(mtm_.get());
            process_tier(ltm_.get());

            // Rebuild lookup tables after potential tier changes so subsequent
            // calls to findBlockByPtr reflect the updated block locations.
            rebuildLookup();
        }

        // Convenience helpers used in tests
        SEPResult MemoryTierManager::promoteBlock(MemoryBlock *block, MemoryBlock *&out_block)
        {
            if (!block || !block->allocated) return SEPResult::INVALID_ARGUMENT;

            // Determine the target tier for promotion
            MemoryTierEnum next_tier;
            if (block->tier == MemoryTierEnum::STM)
                next_tier = MemoryTierEnum::MTM;
            else if (block->tier == MemoryTierEnum::MTM)
                next_tier = MemoryTierEnum::LTM;
            else
                return SEPResult::INVALID_ARGUMENT;

            return promoteToTier(block, next_tier, out_block);
        }

        SEPResult MemoryTierManager::demoteBlock(MemoryBlock *block, MemoryBlock *&out_block)
        {
            if (!block || !block->allocated) return SEPResult::INVALID_ARGUMENT;

            MemoryTierEnum target;
            if (block->tier == MemoryTierEnum::LTM)
                target = MemoryTierEnum::MTM;
            else if (block->tier == MemoryTierEnum::MTM)
                target = MemoryTierEnum::STM;
            else
                return SEPResult::INVALID_ARGUMENT;

            return promoteToTier(block, target, out_block);
        }

        // --- Promotion and Demotion Logic ---
        SEPResult MemoryTierManager::promoteToTier(MemoryBlock *block, MemoryTierEnum target_tier,
                                                   MemoryBlock *&out_block)
        {
            out_block = nullptr;
            printf("DEBUG: Attempting promotion from tier %d to tier %d\n",
                   static_cast<int>(block->tier), static_cast<int>(target_tier));
            if (!block || !block->allocated)
            {
                return SEPResult::INVALID_ARGUMENT;
            }

            // Get source and destination tiers
            MemoryTier *src_tier = getTier(block->tier);
            MemoryTier *dst_tier = getTier(target_tier);
            if (!src_tier || !dst_tier)
            {
                return SEPResult::INVALID_ARGUMENT;
            }

            // Try to allocate in destination tier
            out_block = dst_tier->allocate(block->size);
            if (!out_block)
            {
                printf("DEBUG: Initial allocation failed, attempting defragmentation\n");
                dst_tier->defragment();
                out_block = dst_tier->allocate(block->size);

                // Ensure tier has at least space for the block
                if (!out_block && dst_tier->getSize() < block->size * 2)
                {
                    std::size_t target = std::max(block->size * 2, dst_tier->getSize() * 2);
                    if (dst_tier->resize(target)) out_block = dst_tier->allocate(block->size);
                }
            }

            if (!out_block)
            {
                printf(
                    "DEBUG: Allocation failed even after defragmentation; attempting "
                    "compression\n");
                SEPResult compress_result = compressBlock(block);
                if (compress_result == SEPResult::SUCCESS)
                {
                    out_block = dst_tier->allocate(block->size);
                }
            }

            if (!out_block)
            {
                return SEPResult::OUT_OF_MEMORY;
            }

            // Copy data
            std::memcpy(out_block->ptr, block->ptr, block->size);

            // Copy properties
            out_block->coherence = block->coherence;
            out_block->stability = block->stability;
            out_block->promotion_score = block->promotion_score;
            out_block->priority_score = block->priority_score;
            out_block->age = block->age;
            out_block->generation = block->generation;
            out_block->weight = block->weight;

            // Save entry in the lookup map
            lookup_map_[out_block->ptr] = out_block;

            // Put the old entry in the legacy map temporarily
            legacy_lookup_map_[block->ptr] = out_block;

            // Release the old block
            src_tier->deallocate(block);

            printf("DEBUG: Promotion complete with coherence %.3f, stability %.3f\n",
                   out_block->coherence, out_block->stability);
            return SEPResult::SUCCESS;
        }

        SEPResult MemoryTierManager::compressBlock(MemoryBlock *block)
        {
            if (!block || !block->allocated || !config_.enable_compression)
                return SEPResult::INVALID_ARGUMENT;

            // Real compression would go here
            return SEPResult::NOT_IMPLEMENTED;
        }

        MemoryTier *MemoryTierManager::determineTier(float coherence, float stability,
                                                     int generation_count)
        {
            if (coherence >= config_.promote_mtm_to_ltm &&
                stability >= config_.promote_mtm_to_ltm &&
                static_cast<uint32_t>(generation_count) >= config_.mtm_to_ltm_min_gen)
            {
                return ltm_.get();
            }
            else if (coherence >= config_.promote_stm_to_mtm &&
                     stability >= config_.promote_stm_to_mtm &&
                     static_cast<uint32_t>(generation_count) >= config_.stm_to_mtm_min_gen)
            {
                return mtm_.get();
            }
            else
            {
                return stm_.get();
            }
        }

        // Implementation for updateBlockMetrics
        MemoryBlock *MemoryTierManager::updateBlockMetrics(MemoryBlock *block, float coherence,
                                                           float stability, uint32_t generation,
                                                           float context_score)
        {
            // Guard against invalid input early. Previously this method returned
            // nullptr when passed a stale pointer (for example after a block was
            // promoted and the caller still held the old address). In practice this
            // caused unit tests to fail because the lookup table retains aliases to
            // old pointers for a short period.  To make the behaviour more robust we
            // attempt to resolve the block through findBlockByPtr when the provided
            // pointer no longer refers to an allocated block.
            if (!block || !block->allocated)
            {
                MemoryBlock *resolved = block ? findBlockByPtr(block->ptr) : nullptr;
                if (!resolved || !resolved->allocated) return nullptr;
                block = resolved;
            }

            block->coherence = std::clamp(coherence, 0.0f, 1.0f);
            block->stability = std::clamp(stability, 0.0f, 1.0f);
            block->generation = generation;
            block->weight = context_score;

            // Tier-specific promotion scoring
            switch (block->tier)
            {
                case MemoryTierEnum::STM: {
                    float promotion_threshold = config_.promote_stm_to_mtm;
                    float avg_score = (block->coherence + block->stability) * 0.5f;
                    bool eligible_for_promotion = avg_score >= promotion_threshold &&
                                                  block->generation >= config_.stm_to_mtm_min_gen;
                    block->promotion_score = eligible_for_promotion ? avg_score : 0.0f;
                    block->priority_score = avg_score * (1.0f + block->weight * 0.2f);
                }
                break;
                case MemoryTierEnum::MTM: {
                    float promotion_threshold = config_.promote_mtm_to_ltm;
                    float avg_score = (block->coherence + block->stability) * 0.5f;
                    bool eligible_for_promotion = avg_score >= promotion_threshold &&
                                                  block->generation >= config_.mtm_to_ltm_min_gen;
                    block->promotion_score = eligible_for_promotion ? avg_score : 0.0f;
                    block->priority_score = avg_score * (1.0f + block->weight * 0.3f);
                }
                break;
                case MemoryTierEnum::LTM: {
                    float avg_score = (block->coherence + block->stability) * 0.5f;
                    block->promotion_score = 0.0f;  // Nothing above LTM
                    block->priority_score = avg_score * (1.0f + block->weight * 0.5f);
                }
                break;
                default: {
                    // Handle HOST, DEVICE, UNIFIED or any other memory type
                    float avg_score = (block->coherence + block->stability) * 0.5f;
                    block->promotion_score = 0.0f;
                    block->priority_score = avg_score;
                }
                break;
            }

            // Promote or demote blocks if needed based on their scores
            if (block->promotion_score > 0.0f)
            {
                MemoryTierEnum target_tier;
                if (block->tier == MemoryTierEnum::STM)
                    target_tier = MemoryTierEnum::MTM;
                else if (block->tier == MemoryTierEnum::MTM)
                    target_tier = MemoryTierEnum::LTM;
                else
                    return block;  // No promotion from LTM

                MemoryBlock *new_block = nullptr;
                if (promoteToTier(block, target_tier, new_block) == SEPResult::SUCCESS)
                {
                    return new_block;
                }
            }
            else if (block->coherence < config_.demote_threshold ||
                     block->stability < config_.demote_threshold)
            {
                if (block->tier == MemoryTierEnum::LTM || block->tier == MemoryTierEnum::MTM)
                {
                    MemoryTierEnum target_tier = (block->tier == MemoryTierEnum::LTM)
                                                     ? MemoryTierEnum::MTM
                                                     : MemoryTierEnum::STM;
                    MemoryBlock *new_block = nullptr;
                    if (promoteToTier(block, target_tier, new_block) == SEPResult::SUCCESS)
                    {
                        return new_block;
                    }
                }
            }

            return block;
        }

        // Implementation of the missing updateBlockProperties function
        MemoryBlock *MemoryTierManager::updateBlockProperties(MemoryBlock *block,
                                                              float promotion_score,
                                                              float priority_score,
                                                              std::uint32_t age, float weight)
        {
            // Guard against invalid input early
            if (!block || !block->allocated)
            {
                MemoryBlock *resolved = block ? findBlockByPtr(block->ptr) : nullptr;
                if (!resolved || !resolved->allocated) return nullptr;
                block = resolved;
            }

            // Update block properties
            block->promotion_score = promotion_score;
            block->priority_score = priority_score;
            block->age = age;
            block->weight = weight;

            return block;
        }

        // ----- Relationship Management -----
        void MemoryTierManager::updateGenericRelationship(std::size_t id_a, std::size_t id_b,
                                                          float strength)
        {
            data_relationships_[id_a][id_b] = strength;
        }

        void MemoryTierManager::removeDataEntry(std::size_t id)
        {
            data_registry_.erase(id);
            data_relationships_.erase(id);
            for (auto &rel : data_relationships_)
            {
                rel.second.erase(id);
            }
        }

        void MemoryTierManager::cleanupExpiredData()
        {
            // TODO: Implement
        }

        void MemoryTierManager::pruneDataByPriority([[maybe_unused]] MemoryTierEnum tier,
                                                    [[maybe_unused]] size_t max_count)
        {
            // TODO: Implement
        }

        // ----- Pattern-specific relationship management -----
        void MemoryTierManager::registerPattern(std::size_t id,
                                                const ::sep::pattern::PatternData &pattern)
        {
            pattern_registry_[id] = std::make_unique<::sep::pattern::PatternData>(pattern);
        }

        void MemoryTierManager::registerPattern(std::size_t id,
                                                const ::sep::quantum::Pattern &pattern)
        {
            auto patternData = std::make_unique<::sep::pattern::PatternData>();
            patternData->id = pattern.id;
            patternData->generation = pattern.quantum_state.generation;
            patternData->position = pattern.position;
            patternData->velocity = glm::vec4(pattern.momentum, 0.0f); // Convert vec3 to vec4
            patternData->attributes = glm::vec4(0.0f); // Default attributes
            patternData->amplitude = std::complex<float>(0.0f, 0.0f); // Default amplitude
            patternData->state = pattern.quantum_state.state;
            patternData->phase = pattern.quantum_state.phase;
            patternData->coherence = pattern.quantum_state.coherence;
            patternData->stability = pattern.quantum_state.stability;
            patternData->entropy = pattern.quantum_state.entropy;
            patternData->mutation_rate = pattern.quantum_state.mutation_rate;
            patternData->mutation_count = pattern.quantum_state.mutation_count;
            patternData->memory_tier = pattern.quantum_state.memory_tier;
            patternData->relationships = pattern.relationships;
            patternData->data = pattern.data;
            
            pattern_registry_[id] = std::move(patternData);
        }

        const ::sep::pattern::PatternData *MemoryTierManager::getPatternData(std::size_t id) const
        {
            auto it = pattern_registry_.find(id);
            return it != pattern_registry_.end() ? it->second.get() : nullptr;
        }

        void MemoryTierManager::removePattern(std::size_t id)
        {
            pattern_registry_.erase(id);
            pattern_relationships_.erase(id);
            for (auto &rel : pattern_relationships_)
            {
                rel.second.erase(id);
            }
        }

        void MemoryTierManager::updateRelationship(std::size_t id_a, std::size_t id_b,
                                                   float strength)
        {
            pattern_relationships_[id_a][id_b] = strength;
        }

        void MemoryTierManager::pruneWeakRelationships()
        {
            // TODO: Implement
        }

        void MemoryTierManager::calculateRelationshipScores()
        {
            // TODO: Implement
        }

        void MemoryTierManager::calculateRelationshipCoherence()
        {
            // TODO: Implement
        }

        void MemoryTierManager::cleanupExpiredPatterns()
        {
            // TODO: Implement
        }

        void MemoryTierManager::prunePatternsByPriority([[maybe_unused]] MemoryTierEnum tier,
                                                        [[maybe_unused]] size_t max_count)
        {
            // TODO: Implement
        }

        void MemoryTierManager::loadDataFromPersistence()
        {
            // TODO: Implement
        }

        void MemoryTierManager::storeDataToPersistence(
            [[maybe_unused]] const void *data,
            [[maybe_unused]] const ::sep::persistence::PersistentPatternData &metadata)
        {
            // TODO: Implement
        }

        void *MemoryTierManager::findDataById(std::size_t id)
        {
            auto it = data_registry_.find(id);
            return it != data_registry_.end() ? it->second.get() : nullptr;
        }

        const void *MemoryTierManager::findDataById(std::size_t id) const
        {
            auto it = data_registry_.find(id);
            return it != data_registry_.end() ? it->second.get() : nullptr;
        }

        void MemoryTierManager::registerGenericData([[maybe_unused]] std::size_t id,
                                                    [[maybe_unused]] const void *data)
        {
            // TODO: Implement properly with proper cloning
        }

        const void *MemoryTierManager::getRegisteredData(std::size_t id) const
        {
            return findDataById(id);
        }

        void MemoryTierManager::resetForTesting(const Config &cfg)
        {
            shutdown();
            stm_->resize(cfg.stm_size > 0 ? cfg.stm_size : 1 << 20);
            mtm_->resize(cfg.mtm_size > 0 ? cfg.mtm_size : 4 << 20);
            ltm_->resize(cfg.ltm_size > 0 ? cfg.ltm_size : 16 << 20);
            init(cfg);
        }

        SEPResult MemoryTierManager::processMemoryBlocks(void *input_data, void *output_data,
                                                         const void *config, size_t count,
                                                         const void *previous_data, void *stream)
        {
            // TODO: Implement
            (void)input_data;
            (void)output_data;
            (void)config;
            (void)count;
            (void)previous_data;
            (void)stream;
            return SEPResult::NOT_IMPLEMENTED;
        }

    }  // namespace memory

    namespace config
    {
        void to_json(nlohmann::json &j, const MemoryThresholdConfig &c)
        {
            j = nlohmann::json{{"promote_stm_to_mtm", c.promote_stm_to_mtm},
                               {"promote_mtm_to_ltm", c.promote_mtm_to_ltm},
                               {"demote_threshold", c.demote_threshold},
                               {"fragmentation_threshold", c.fragmentation_threshold},
                               {"stm_size", c.stm_size},
                               {"mtm_size", c.mtm_size},
                               {"ltm_size", c.ltm_size},
                               {"stm_to_mtm_min_gen", c.stm_to_mtm_min_gen},
                               {"mtm_to_ltm_min_gen", c.mtm_to_ltm_min_gen},
                               {"use_unified_memory", c.use_unified_memory},
                               {"enable_compression", c.enable_compression}};
        }

        void from_json(const nlohmann::json &j, MemoryThresholdConfig &c)
        {
            j.at("promote_stm_to_mtm").get_to(c.promote_stm_to_mtm);
            j.at("promote_mtm_to_ltm").get_to(c.promote_mtm_to_ltm);
            j.at("demote_threshold").get_to(c.demote_threshold);
            j.at("fragmentation_threshold").get_to(c.fragmentation_threshold);
            j.at("stm_size").get_to(c.stm_size);
            j.at("mtm_size").get_to(c.mtm_size);
            j.at("ltm_size").get_to(c.ltm_size);
            j.at("stm_to_mtm_min_gen").get_to(c.stm_to_mtm_min_gen);
            j.at("mtm_to_ltm_min_gen").get_to(c.mtm_to_ltm_min_gen);
            j.at("use_unified_memory").get_to(c.use_unified_memory);
            j.at("enable_compression").get_to(c.enable_compression);
        }
    }  // namespace config
}  // namespace sep
