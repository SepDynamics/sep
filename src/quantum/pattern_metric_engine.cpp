#include "quantum/pattern_metric_engine.h"

namespace sep::quantum {

PatternMetricEngine::PatternMetricEngine() : qfh_processor_(std::make_unique<QuantumProcessorQFH>()) {}

void PatternMetricEngine::clear() {
    std::lock_guard<std::mutex> lock(engine_mutex_);
    current_patterns_.clear();
    current_metrics_.clear();
    stream_buffer_.clear();
    if (qfh_processor_) {
        qfh_processor_->clear();
    }
}

SEPResult PatternMetricEngine::init([[maybe_unused]] quantum::GPUContext* ctx) {
    return SEPResult::SUCCESS;
}

void PatternMetricEngine::ingestData(const uint8_t* data, size_t size) {
    std::lock_guard<std::mutex> lock(engine_mutex_);
    stream_buffer_.insert(stream_buffer_.end(), data, data + size);
}

void PatternMetricEngine::ingestData(std::istream& stream) {
    std::vector<uint8_t> buffer(4096);
    while (stream.read(reinterpret_cast<char*>(buffer.data()), buffer.size())) {
        ingestData(buffer.data(), stream.gcount());
    }
    ingestData(buffer.data(), stream.gcount());
}

void PatternMetricEngine::ingestFile(const std::string& filepath) {
    std::ifstream file(filepath, std::ios::binary);
    if (file) {
        ingestData(file);
    }
}

void PatternMetricEngine::ingestMappedFile([[maybe_unused]] const std::string& filepath) {
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

void PatternMetricEngine::addPattern(const pattern::PatternData& pattern) {
    std::lock_guard<std::mutex> lock(engine_mutex_);
    current_patterns_.push_back(pattern);
}

pattern::PatternData PatternMetricEngine::mutatePattern(const pattern::PatternData& parent) {
    pattern::PatternData mutated = parent;
    mutated.id = parent.id + "_child";
    mutated.generation++;
    if (!mutated.data.empty()) {
        mutated.data[0] += 0.1f;
    }
    return mutated;
}

std::vector<PatternMetrics> PatternMetricEngine::computeMetrics() {
    std::lock_guard<std::mutex> lock(engine_mutex_);
    
    // PERFORMANCE FIX: Clear the processor state before the metric run.
    // This prevents the O(N^2) slowdown from an ever-growing history in the processor.
    // Each call to computeMetrics is now an independent analysis of the current patterns.
    qfh_processor_->clear();
    
    current_metrics_.clear();
    current_metrics_.reserve(current_patterns_.size());

    for (const auto& p : current_patterns_) {
        PatternMetrics m;
        if (qfh_processor_ && !p.data.empty()) {
            if (p.data.size() >= 3) {
                m.coherence = qfh_processor_->processPattern(glm::vec3(p.data[0], p.data[1], p.data[2]));
            } else {
                m.coherence = qfh_processor_->processPattern(glm::vec3(p.data[0], 0.0f, 0.0f));
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

const std::vector<pattern::PatternData>& PatternMetricEngine::getPatterns() const {
    return current_patterns_;
}

std::vector<pattern::PatternData> PatternMetricEngine::extractPatternsFromBytes(const uint8_t* data, size_t size) {
    std::vector<pattern::PatternData> patterns;
    const size_t float_size = sizeof(float);
    const size_t chunk_size_floats = 16;
    const size_t chunk_size_bytes = chunk_size_floats * float_size;

    if (size == 0) {
        return patterns;
    }

    size_t num_patterns = size / chunk_size_bytes;
    for (size_t i = 0; i < num_patterns; ++i) {
        pattern::PatternData p;
        p.id = "pattern_" + std::to_string(i);
        
        const float* float_data = reinterpret_cast<const float*>(data + (i * chunk_size_bytes));
        p.data.assign(float_data, float_data + chunk_size_floats);
        
        patterns.push_back(p);
    }

    size_t remaining_bytes = size % chunk_size_bytes;
    if (remaining_bytes > 0) {
        pattern::PatternData p;
        p.id = "pattern_" + std::to_string(num_patterns);
        
        const uint8_t* remaining_data_ptr = data + (num_patterns * chunk_size_bytes);
        std::vector<float> float_vec;
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