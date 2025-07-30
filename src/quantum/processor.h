#ifndef SEP_QUANTUM_PROCESSOR_H
#define SEP_QUANTUM_PROCESSOR_H
#pragma once

#include <memory>
#include <string>
#include <vector>

#include "engine/internal/common.h"
#include "engine/internal/pattern_types.h"
#include "engine/internal/system_hooks.h"
#include "engine/internal/types.h"
#include "quantum/gpu_context.h"
#include "quantum/types.h"

namespace sep {

// Constants for pattern processing
namespace quantum {

constexpr float MIN_COHERENCE = 0.1f;
constexpr float STABILITY_THRESHOLD = 0.85f;
constexpr float COHERENCE_THRESHOLD = 0.7f;
constexpr float DEMOTION_THRESHOLD = 0.3f;
} // namespace quantum

// Result objects returned by Processor operations
struct ProcessingResult {
    bool success{false};
    compat::Pattern pattern{};
    std::string error_message{};
};

struct BatchProcessingResult : public ProcessingResult {
    std::vector<ProcessingResult> results{};
};

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

    virtual sep::SEPResult init(quantum::GPUContext* ctx);
    virtual void evolvePatterns();
    virtual compat::PatternData mutatePattern(const compat::PatternData& parent);

    sep::SEPResult addPattern(const compat::PatternData& pattern);
    const std::vector<compat::PatternData>& getPatterns() const;

    // Convenience method to evolve patterns and return the results
    virtual const std::vector<compat::PatternData>& process()
    {
        evolvePatterns();
        return patterns_;
    }
    
protected:
    Implementation implementation_;
    std::vector<compat::PatternData> patterns_;
};

// CPU implementation of pattern processor
class CPUPatternProcessor : public PatternProcessor {
public:
    explicit CPUPatternProcessor();
    ~CPUPatternProcessor() override = default;

    sep::SEPResult init(quantum::GPUContext* ctx) override;
    void evolvePatterns() override;
    compat::PatternData mutatePattern(const compat::PatternData& parent) override;
    
protected:
    std::vector<compat::PatternData>& patterns_;
};

} // namespace pattern

namespace quantum {
namespace core { class SystemHooks; }

class ProcessorImpl;

// Processor is thread-safe. All mutations of internal state are
// guarded by an internal mutex within the implementation.
class Processor {
public:
    explicit Processor(const ProcessingConfig& config);
    ~Processor();
    Processor(Processor&&) noexcept;
    Processor& operator=(Processor&&) noexcept;

    sep::SEPResult init(GPUContext* gpu_context);
    void setHooks(core::SystemHooks* hooks);

    sep::SEPResult addPattern(const compat::Pattern& pattern);
    sep::SEPResult removePattern(const std::string& pattern_id);
    sep::SEPResult updatePattern(const std::string& pattern_id, const compat::Pattern& pattern);
    compat::Pattern getPattern(const std::string& pattern_id) const;
    std::vector<sep::compat::Pattern> getPatterns() const;
    std::vector<sep::compat::Pattern> getPatternsByTier(sep::memory::MemoryTierEnum tier) const;
    size_t getPatternCount() const;

    ProcessingResult processPattern(const std::string& pattern_id);
    BatchProcessingResult processBatch(const std::vector<std::string>& pattern_ids);
    BatchProcessingResult processAll();

    ProcessingResult evolvePattern(const std::string& pattern_id);
    ProcessingResult collapsePattern(const std::string& pattern_id);
    ProcessingResult entanglePatterns(const std::string& pattern_id1,
                                      const std::string& pattern_id2);
    ProcessingResult mutatePattern(const std::string& parent_id);

    void promotePatterns();
    void demotePatterns();
    void removeWeakPatterns();

    sep::SEPResult addRelationship(const std::string& pattern_id1, const std::string& pattern_id2,
                                   float strength, RelationshipType type);
    float calculateCoherence(const std::string& pattern_id1, const std::string& pattern_id2) const;

    std::string getStatus() const;
    ProcessingConfig getConfig() const;
    void updateConfig(const ProcessingConfig& config);

private:
    // All mutable state lives inside ProcessorImpl which uses a mutex internally
    std::unique_ptr<ProcessorImpl> impl_;
};

std::unique_ptr<Processor> createProcessor(const ProcessingConfig& config);
std::unique_ptr<Processor> createProcessor();
std::unique_ptr<Processor> createCPUProcessor(const ProcessingConfig& config);
std::unique_ptr<Processor> createGPUProcessor(const ProcessingConfig& config);

} // namespace quantum
} // namespace sep

#endif // SEP_QUANTUM_PROCESSOR_H