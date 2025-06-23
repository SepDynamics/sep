#ifndef SEP_QUANTUM_PROCESSOR_H
#define SEP_QUANTUM_PROCESSOR_H

#include "quantum/types.h"
#include "core/common.h"
#include <memory>
#include <vector>

namespace sep::quantum {

class GPUContext;
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
    std::vector<Pattern> getPatterns() const;
    std::vector<Pattern> getPatternsByTier(MemoryTierEnum tier) const;
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

} // namespace sep::quantum

#endif // SEP_QUANTUM_PROCESSOR_H