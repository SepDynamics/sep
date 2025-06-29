#ifndef SEP_QUANTUM_PROCESSOR_H
#define SEP_QUANTUM_PROCESSOR_H
#pragma once

#include "types.h"
#include "core/common.h"
#include "core/system_hooks.h"
// Removed duplicate include of core/types.h
#include "quantum/gpu_context.h"
#include "quantum/data.hpp"
#include "memory/types.h"
#include <memory>
#include <vector>
#include <string>

namespace sep {

// Constants for pattern processing
namespace quantum {
constexpr float MIN_COHERENCE = 0.1f;
constexpr float STABILITY_THRESHOLD = 0.85f;
constexpr float COHERENCE_THRESHOLD = 0.7f;
constexpr float DEMOTION_THRESHOLD = 0.3f;
} // namespace quantum

namespace pattern {

// Base pattern processor class
class PatternProcessor {
public:
    enum class Implementation {
        CPU,
        GPU,
        QUANTUM
    };

    explicit PatternProcessor(Implementation impl = Implementation::CPU);
    virtual ~PatternProcessor() = default;

    virtual SEPResult init(quantum::GPUContext* ctx);
    virtual void evolvePatterns();
    virtual PatternData mutatePattern(const PatternData& parent);
    
    SEPResult addPattern(const PatternData& pattern);
    const std::vector<PatternData>& getPatterns() const;

    // Convenience method to evolve patterns and return the results
    virtual const std::vector<PatternData>& process()
    {
        evolvePatterns();
        return patterns_;
    }
    
protected:
    Implementation implementation_;
    std::vector<PatternData> patterns_;
};

// CPU implementation of pattern processor
class CPUPatternProcessor : public PatternProcessor {
public:
    explicit CPUPatternProcessor();
    ~CPUPatternProcessor() override = default;

    SEPResult init(quantum::GPUContext* ctx) override;
    void evolvePatterns() override;
    PatternData mutatePattern(const PatternData& parent) override;
    
protected:
    std::vector<PatternData>& patterns_;
};

} // namespace pattern

namespace quantum {
namespace core { class SystemHooks; }

class ProcessorImpl;

class Processor {
public:
    explicit Processor(const ProcessingConfig& config);
    ~Processor();
    Processor(Processor&&) noexcept;
    Processor& operator=(Processor&&) noexcept;

    SEPResult init(GPUContext* gpu_context);
    void setHooks(core::SystemHooks* hooks);

    SEPResult addPattern(const Pattern& pattern);
    SEPResult removePattern(const std::string& pattern_id);
    SEPResult updatePattern(const std::string& pattern_id, const Pattern& pattern);
    Pattern getPattern(const std::string& pattern_id) const;
    std::vector<sep::quantum::Pattern> getPatterns() const;
    std::vector<sep::quantum::Pattern> getPatternsByTier(::sep::memory::MemoryTierEnum tier) const;
    size_t getPatternCount() const;

    ProcessingResult processPattern(const std::string& pattern_id);
    BatchProcessingResult processBatch(const std::vector<std::string>& pattern_ids);
    BatchProcessingResult processAll();

    ProcessingResult evolvePattern(const std::string& pattern_id);
    ProcessingResult collapsePattern(const std::string& pattern_id);
    ProcessingResult entanglePatterns(const std::string& pattern_id1, const std::string& pattern_id2);
    ProcessingResult mutatePattern(const std::string& parent_id);

    void promotePatterns();
    void demotePatterns();
    void removeWeakPatterns();

    SEPResult addRelationship(const std::string& pattern_id1, const std::string& pattern_id2,
                              float strength, RelationshipType type);
    float calculateCoherence(const std::string& pattern_id1, const std::string& pattern_id2) const;

    std::string getStatus() const;
    ProcessingConfig getConfig() const;
    void updateConfig(const ProcessingConfig& config);

private:
    std::unique_ptr<ProcessorImpl> impl_;
};

std::unique_ptr<Processor> createProcessor(const ProcessingConfig& config);
std::unique_ptr<Processor> createCPUProcessor(const ProcessingConfig& config);
std::unique_ptr<Processor> createGPUProcessor(const ProcessingConfig& config);

} // namespace quantum
} // namespace sep

#endif // SEP_QUANTUM_PROCESSOR_H