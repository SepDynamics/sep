#pragma once

#include <cstddef>
#include <vector>

namespace sep::memory
{

    enum class MemoryTierEnum
    {
        STM,
        MTM,
        LTM
    };

    struct MemoryBlock
    {
        void* ptr;
        std::size_t size;
        std::size_t offset;
        MemoryTierEnum tier;
        bool allocated;
        float coherence;
        float stability;
        int generation;
        float weight;
    };

    class MemoryTier
    {
    public:
        struct Config
        {
            std::size_t size;
            MemoryTierEnum type;
        };

        MemoryTier(const Config& config);
        ~MemoryTier();

        MemoryBlock* allocate(std::size_t size);
        void deallocate(MemoryBlock* block);
        float calculateUtilization() const;
        std::size_t getUsedSpace() const { return used_space_; }
        std::size_t getSize() const { return config_.size; }
        bool resize(std::size_t new_size);
        bool moveData(MemoryBlock* dst, const MemoryBlock* src);
        void defragment();
        const std::vector<MemoryBlock>& getBlocks() const { return blocks_; }

    private:
        Config config_;
        void* memory_pool_;
        std::size_t used_space_;
        std::vector<MemoryBlock> blocks_;
    };

}  // namespace sep::memory