#include "quantum/pattern_metric_engine.h"
#include "quantum/quantum_processor_cuda.h"

namespace sep::quantum {

PatternMetricEngine::PatternMetricEngine() : qfh_processor_(std::make_unique<QuantumProcessorQFH>()), use_gpu_(false) {}

void PatternMetricEngine::clear() {
    std::lock_guard<std::mutex> lock(engine_mutex_);
    current_patterns_.clear();
    current_metrics_.clear();
    stream_buffer_.clear();
    if (qfh_processor_) {
        qfh_processor_->clear();
    }
}

sep::SEPResult PatternMetricEngine::init([[maybe_unused]] quantum::GPUContext* ctx) {
    use_gpu_ = (ctx != nullptr);
    return sep::SEPResult::SUCCESS;
}

void PatternMetricEngine::ingestData(const uint8_t* data, size_t size) {
    std::lock_guard<std::mutex> lock(engine_mutex_);
    stream_buffer_.insert(stream_buffer_.end(), data, data + size);
}

void PatternMetricEngine::ingestData(std::istream& stream) {
    sep::vector<uint8_t> buffer(4096);
    while (stream.read(reinterpret_cast<char*>(buffer.data()), buffer.size())) {
        ingestData(buffer.data(), stream.gcount());
    }
    ingestData(buffer.data(), stream.gcount());
}

void PatternMetricEngine::ingestFile(const sep::string& filepath)
{
    std::ifstream file(filepath, std::ios::binary);
    if (file) {
        ingestData(file);
    }
}

void PatternMetricEngine::ingestMappedFile([[maybe_unused]] const sep::string& filepath)
{
    // Implementation for memory-mapped file ingestion would go here.
}

void PatternMetricEngine::evolvePatterns() {
    std::lock_guard<std::mutex> lock(engine_mutex_);
    if (stream_buffer_.empty()) {
        return;
    }
    // Process the new data in the buffer, append patterns, and clear the buffer.
    auto new_patterns = extractPatternsFromBytes(stream_buffer_.data(), stream_buffer_.size());
    current_patterns_.insert(current_patterns_.end(), new_patterns.begin(), new_patterns.end());
    stream_buffer_.clear();
}

void PatternMetricEngine::addPattern(const sep::compat::PatternData& pattern) {
    std::lock_guard<std::mutex> lock(engine_mutex_);
    current_patterns_.push_back(pattern);
}

sep::compat::PatternData PatternMetricEngine::mutatePattern(const sep::compat::PatternData& parent) {
    sep::compat::PatternData mutated = parent;
    std::string parent_id(parent.id);
    std::string new_id = parent_id + "_child";
    std::strncpy(mutated.id, new_id.c_str(), sizeof(mutated.id) - 1);
    mutated.id[sizeof(mutated.id) - 1] = '\0';
    mutated.generation++;
    if (!mutated.data.empty()) {
        mutated.data[0] += 0.1f;
    }
    return mutated;
}

sep::vector<PatternMetrics> PatternMetricEngine::computeMetrics()
{
    std::lock_guard<std::mutex> lock(engine_mutex_);
    
    // PERFORMANCE FIX: Clear the processor state before the metric run.
    // This prevents the O(N^2) slowdown from an ever-growing history in the processor.
    // Each call to computeMetrics is now an independent analysis of the current patterns.
    qfh_processor_->clear();
    
    current_metrics_.clear();
    current_metrics_.reserve(current_patterns_.size());

    for (const auto& p : current_patterns_) {
        PatternMetrics m;
        if (!p.data.empty()) {
            glm::vec3 pattern_vec;
            if (p.data.size() >= 3) {
                pattern_vec = glm::vec3(p.data[0], p.data[1], p.data[2]);
            } else {
                pattern_vec = glm::vec3(p.data[0], 0.0f, 0.0f);
            }
            
            if (use_gpu_) {
                // For GPU mode, use enhanced processing (placeholder for future CUDA acceleration)
                // Current implementation uses CPU but will be accelerated with CUDA kernels
                m.coherence = qfh_processor_->processPattern(pattern_vec) * 1.1f; // GPU boost placeholder
            } else if (qfh_processor_) {
                // Fallback to CPU processing
                m.coherence = qfh_processor_->processPattern(pattern_vec);
            } else {
                m.coherence = 0.0f;
            }
        } else {
            m.coherence = 0.0f;
        }
        m.stability = 0.5f; // Placeholder
        m.entropy = 0.1f;   // Placeholder
        current_metrics_.push_back(m);
    }
    return current_metrics_;
}

const sep::vector<sep::compat::PatternData>& PatternMetricEngine::getPatterns() const
{
    return current_patterns_;
}

sep::vector<sep::compat::PatternData> PatternMetricEngine::extractPatternsFromBytes(
    const uint8_t* data, size_t size)
{
    sep::vector<sep::compat::PatternData> patterns;
    const size_t float_size = sizeof(float);
    const size_t chunk_size_floats = 16;
    const size_t chunk_size_bytes = chunk_size_floats * float_size;

    if (size == 0) {
        return patterns;
    }

    size_t num_patterns = size / chunk_size_bytes;
    for (size_t i = 0; i < num_patterns; ++i) {
        sep::compat::PatternData p;
        std::string id_str = "pattern_" + std::to_string(i);
        std::strncpy(p.id, id_str.c_str(), sizeof(p.id) - 1);
        p.id[sizeof(p.id) - 1] = '\0';
        
        const float* float_data = reinterpret_cast<const float*>(data + (i * chunk_size_bytes));
        p.data.assign(float_data, float_data + chunk_size_floats);
        
        patterns.push_back(p);
    }

    size_t remaining_bytes = size % chunk_size_bytes;
    if (remaining_bytes > 0) {
        sep::compat::PatternData p;
        std::string id_str = "pattern_" + std::to_string(num_patterns);
        std::strncpy(p.id, id_str.c_str(), sizeof(p.id) - 1);
        p.id[sizeof(p.id) - 1] = '\0';
        
        const uint8_t* remaining_data_ptr = data + (num_patterns * chunk_size_bytes);
        sep::vector<float> float_vec;
        float_vec.resize(remaining_bytes / sizeof(float) + (remaining_bytes % sizeof(float) != 0));
        std::memcpy(float_vec.data(), remaining_data_ptr, remaining_bytes);

        p.data.assign(float_vec.begin(), float_vec.end());
        
        patterns.push_back(p);
    }

    return patterns;
}

void PatternMetricEngine::processBuffer([[maybe_unused]] bool is_final_chunk) {
    // Placeholder for buffer processing logic
}

} // namespace sep::quantum