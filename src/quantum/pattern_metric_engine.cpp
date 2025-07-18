#include "quantum/pattern_metric_engine.h"
#include "core/common.h"
#include <algorithm>
#include <cstring>
#include <sstream>
#include <fstream>
#include <functional>

#ifdef __linux__
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#endif

namespace sep::quantum {

PatternMetricEngine::PatternMetricEngine()
    : pattern::PatternProcessor(Implementation::QUANTUM),
      qfh_processor_(std::make_unique<QuantumProcessorQFH>()) {
}

SEPResult PatternMetricEngine::init(quantum::GPUContext* ctx) {
    auto result = PatternProcessor::init(ctx);
    if (result != SEPResult::SUCCESS) {
        return result;
    }
    
    current_patterns_.clear();
    current_metrics_.clear();
    return SEPResult::SUCCESS;
}

void PatternMetricEngine::ingestData(const uint8_t* data, size_t size) {
    if (!data || size == 0) {
        return;
    }

    auto extracted_patterns = extractPatternsFromBytes(data, size);
    for (const auto& pattern : extracted_patterns) {
        addPattern(pattern);
    }
}

void PatternMetricEngine::ingestData(std::istream& stream) {
    const size_t buffer_size = 4096; // Use a smaller chunk size for general streams
    std::vector<uint8_t> read_buffer(buffer_size);

    while (stream.read(reinterpret_cast<char*>(read_buffer.data()), buffer_size)) {
        std::lock_guard<std::mutex> lock(engine_mutex_);
        stream_buffer_.insert(stream_buffer_.end(), read_buffer.begin(), read_buffer.end());
        processBuffer();
    }

    // Handle the last partial read
    if (stream.gcount() > 0) {
        std::lock_guard<std::mutex> lock(engine_mutex_);
        stream_buffer_.insert(stream_buffer_.end(), read_buffer.begin(), read_buffer.begin() + stream.gcount());
    }

    // Process any remaining data in the buffer
    std::lock_guard<std::mutex> lock(engine_mutex_);
    processBuffer(true); // Final chunk
}

void PatternMetricEngine::addPattern(const pattern::PatternData& pattern) {
    patterns_.push_back(pattern);
    current_patterns_.push_back(pattern);
}

std::vector<pattern::PatternData> PatternMetricEngine::extractPatternsFromBytes(
    const uint8_t* data, size_t size) {
    std::vector<pattern::PatternData> patterns;
    
    const size_t chunk_size = 12;  // 3 floats * 4 bytes
    
    if (size < chunk_size) {
        return patterns;
    }

    for (size_t i = 0; i <= size - chunk_size; i += chunk_size) {
        pattern::PatternData p;
        
        glm::vec3 v(0.0f);
        memcpy(&v, data + i, chunk_size);
        
        p.id = "pattern_" + std::to_string(current_patterns_.size() + patterns.size());
        p.generation = 0;
        p.timestamp = std::time(nullptr);
        p.last_accessed = p.timestamp;
        p.last_modified = p.timestamp;

        // Store the vec3 components in the pattern data
        v = glm::normalize(v + glm::vec3(1e-6f));
        p.data.push_back(v.x);
        p.data.push_back(v.y);
        p.data.push_back(v.z);

        // Initialize quantum state
        p.quantum_state.coherence = 0.5f;
        p.quantum_state.stability = 0.5f;
        p.quantum_state.entropy = 0.0f;
        p.quantum_state.state = quantum::QuantumState::Status::SUPERPOSITION;
        
        patterns.push_back(p);
    }
    
    return patterns;
}

void PatternMetricEngine::evolvePatterns() {
    if (patterns_.empty()) {
        return;
    }

    // Process each pattern through QFH
    for (auto& pattern : patterns_) {
        // Convert pattern data to 3D vector for QFH processing
        glm::vec3 pattern_vec(0.0f);
        if (!pattern.data.empty()) {
            pattern_vec.x = pattern.data[0];
            pattern_vec.y = pattern.data.size() > 1 ? pattern.data[1] : 0.0f;
            pattern_vec.z = pattern.data.size() > 2 ? pattern.data[2] : 0.0f;
        }
        
        // Process through QFH
        float coherence = qfh_processor_->processPattern(pattern_vec);
        bool is_stable = qfh_processor_->isStable(pattern_vec);
        
        // Update quantum state
        pattern.quantum_state.coherence = coherence;
        pattern.quantum_state.stability = qfh_processor_->calculateStability(
            pattern_vec,
            pattern.quantum_state.stability,
            pattern.generation,
            pattern.quantum_state.access_frequency
        );
        pattern.quantum_state.entropy = qfh_processor_->getLastQFHResult().entropy;
        
        // Update state based on stability
        if (is_stable) {
            pattern.quantum_state.state = quantum::QuantumState::Status::COHERENT;
        }
        
        pattern.generation++;
    }
}

pattern::PatternData PatternMetricEngine::mutatePattern(const pattern::PatternData& parent) {
    pattern::PatternData mutated = parent;
    mutated.id = "pattern_" + std::to_string(current_patterns_.size());
    mutated.generation = parent.generation + 1;
    mutated.parent_ids.push_back(parent.id);
    
    // Convert to vec3 for QFH mutation
    glm::vec3 pattern_vec(0.0f);
    if (!parent.data.empty()) {
        pattern_vec.x = parent.data[0];
        pattern_vec.y = parent.data.size() > 1 ? parent.data[1] : 0.0f;
        pattern_vec.z = parent.data.size() > 2 ? parent.data[2] : 0.0f;
    }
    
    // Apply quantum mutation
    glm::vec3 mutated_vec = qfh_processor_->mutatePattern(
        pattern_vec,
        0.01f, // Base mutation rate
        parent.quantum_state.mutation_count,
        static_cast<int>(parent.quantum_state.stability * 100)
    );
    
    // Update pattern data with mutated values
    mutated.data.clear();
    mutated.data.push_back(mutated_vec.x);
    mutated.data.push_back(mutated_vec.y);
    mutated.data.push_back(mutated_vec.z);
    
    mutated.quantum_state.mutation_count++;
    mutated.timestamp = std::time(nullptr);
    mutated.last_accessed = mutated.timestamp;
    mutated.last_modified = mutated.timestamp;
    
    return mutated;
}

std::vector<PatternMetrics> PatternMetricEngine::computeMetrics() {
    std::vector<PatternMetrics> metrics;
    metrics.reserve(patterns_.size());
    
    for (const auto& pattern : patterns_) {
        PatternMetrics m;
        m.coherence = pattern.quantum_state.coherence;
        m.stability = pattern.quantum_state.stability;
        m.entropy = pattern.quantum_state.entropy;
        m.relationships = pattern.relationships;
        metrics.push_back(m);
    }
    
    current_metrics_ = metrics;
    return metrics;
}

void PatternMetricEngine::ingestFile(const std::string& filepath) {
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        // TODO: Log error
        return;
    }

    const size_t buffer_size = 4 * 1024 * 1024; // 4MB chunks
    std::vector<uint8_t> read_buffer(buffer_size);

    while (file.read(reinterpret_cast<char*>(read_buffer.data()), buffer_size)) {
        std::lock_guard<std::mutex> lock(engine_mutex_);
        stream_buffer_.insert(stream_buffer_.end(), read_buffer.begin(), read_buffer.end());
        processBuffer();
    }

    if (file.gcount() > 0) {
        std::lock_guard<std::mutex> lock(engine_mutex_);
        stream_buffer_.insert(stream_buffer_.end(), read_buffer.begin(), read_buffer.begin() + file.gcount());
    }

    std::lock_guard<std::mutex> lock(engine_mutex_);
    processBuffer(true);
}

void PatternMetricEngine::ingestMappedFile(const std::string& filepath) {
#ifdef __linux__
    int fd = open(filepath.c_str(), O_RDONLY);
    if (fd == -1) {
        // TODO: Log error
        return;
    }

    struct stat sb;
    if (fstat(fd, &sb) == -1) {
        close(fd);
        // TODO: Log error
        return;
    }

    size_t length = sb.st_size;
    if (length == 0) {
        close(fd);
        return;
    }
    
    const uint8_t* addr = static_cast<const uint8_t*>(mmap(NULL, length, PROT_READ, MAP_PRIVATE, fd, 0));
    if (addr == MAP_FAILED) {
        close(fd);
        // TODO: Log error
        return;
    }

    // With mmap, we can process the whole file at once without chunking in this engine.
    // The OS handles the paging. This is a different approach than chunked reading.
    ingestData(addr, length);

    munmap((void*)addr, length);
    close(fd);
#else
    // Fallback for non-Linux systems
    ingestFile(filepath);
#endif
}

void PatternMetricEngine::processBuffer(bool is_final_chunk) {
    constexpr size_t CHUNK_SIZE = 64;
    
    size_t buffer_size = stream_buffer_.size();
    if (buffer_size < CHUNK_SIZE) {
        if (is_final_chunk) {
            stream_buffer_.clear(); // Not enough for a pattern, discard.
        }
        return; // Not enough data, wait for more.
    }

    // Process the entire buffer with a sliding window.
    // The cache will prevent re-adding duplicate patterns.
    auto extracted_patterns = extractPatternsFromBytes(stream_buffer_.data(), buffer_size);
    for (const auto& pattern : extracted_patterns) {
        addPattern(pattern);
    }

    if (is_final_chunk) {
        stream_buffer_.clear();
    } else {
        // Erase the portion of the buffer that has been fully processed,
        // leaving the tail for overlap analysis with the next chunk.
        size_t erase_count = buffer_size - (CHUNK_SIZE - 1);
        stream_buffer_.erase(stream_buffer_.begin(), stream_buffer_.begin() + erase_count);
    }
}

} // namespace sep::quantum