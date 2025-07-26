#include "quantum/pattern_metric_engine.h"
#include "engine/logging.h"

char sep::quantum::pattern_id[sep::compat::PatternData::MAX_ID_LENGTH];
float sep::quantum::coherence = 0.0f;
float sep::quantum::stability = 0.0f;
float sep::quantum::entropy = 0.0f;
std::vector<sep::quantum::PatternRelationship> sep::quantum::relationships;
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
    std::vector<uint8_t> buffer(4096);
    while (stream.read(reinterpret_cast<char*>(buffer.data()), buffer.size())) {
        ingestData(buffer.data(), stream.gcount());
    }
    ingestData(buffer.data(), stream.gcount());
}

void PatternMetricEngine::ingestFile(const std::string& filepath)
{
    std::ifstream file(filepath, std::ios::binary);
    if (file) {
        ingestData(file);
    }
}

void PatternMetricEngine::ingestMappedFile([[maybe_unused]] const std::string& filepath)
{
    // Implementation for memory-mapped file ingestion would go here.
}

void PatternMetricEngine::evolvePatterns() {
    std::lock_guard<std::mutex> lock(engine_mutex_);
    if (stream_buffer_.empty()) {
        return;
    }
    
    // Use the QFH processor to analyze the raw data
    if (qfh_processor_) {
        // Convert bytes to uint32_t for QFH processing
        std::vector<uint32_t> pattern_bits;
        pattern_bits.reserve((stream_buffer_.size() + 3) / 4); // Reserve space for uint32_t conversion
        
        for (size_t i = 0; i < stream_buffer_.size(); i += 4) {
            uint32_t value = 0;
            for (size_t j = 0; j < 4 && (i + j) < stream_buffer_.size(); ++j) {
                value |= (static_cast<uint32_t>(stream_buffer_[i + j]) << (j * 8));
            }
            pattern_bits.push_back(value);
        }
        
        // This will trigger the QFH analysis which logs "analyze: events size: X"
        qfh_processor_->processPatternBits(pattern_bits);
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

void PatternMetricEngine::setSignalThresholds(const SignalThresholds& thresholds) {
    std::lock_guard<std::mutex> lock(engine_mutex_);
    signal_thresholds_ = thresholds;
}

const std::vector<Signal>& PatternMetricEngine::getSignals() const {
    return current_signals_;
}

const std::vector<PatternMetrics>& PatternMetricEngine::computeMetrics()
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
        std::strncpy(m.pattern_id, p.id, sizeof(m.pattern_id) - 1);
        m.pattern_id[sizeof(m.pattern_id) - 1] = '\0';

        if (!p.data.empty()) {
            // Calculate coherence based on pattern self-similarity and consistency
            float sum_squares = 0.0f;
            float mean = 0.0f;
            
            // Calculate mean
            for (float val : p.data) {
                mean += val;
            }
            mean /= p.data.size();
            
            // Calculate variance and coherence
            float variance = 0.0f;
            for (float val : p.data) {
                float diff = val - mean;
                variance += diff * diff;
                sum_squares += val * val;
            }
            variance /= p.data.size();
            
            // Coherence is high when variance is low relative to signal strength
            // Use coefficient of variation (inverse) as coherence measure
            if (std::abs(mean) > 1e-6f) {
                float cv = std::sqrt(variance) / std::abs(mean);
                m.coherence = std::max(0.0f, std::min(1.0f, 1.0f / (1.0f + cv)));
            } else {
                // For zero-mean patterns, use energy concentration
                float energy = sum_squares / p.data.size();
                m.coherence = std::max(0.0f, std::min(1.0f, energy / (energy + variance + 1e-6f)));
            }
            
            // Boost coherence if pattern has clear structure
            if (p.data.size() >= 4) {
                // Check for repeating patterns or structure
                float structure_bonus = 0.0f;
                for (size_t i = 0; i < p.data.size() - 1; ++i) {
                    if (std::abs(p.data[i] - p.data[i+1]) < 0.1f) {
                        structure_bonus += 0.05f;
                    }
                }
                m.coherence = std::min(1.0f, m.coherence + structure_bonus);
            }
        } else {
            m.coherence = 0.0f;
        }
        
        // Calculate stability based on pattern consistency
        if (!p.data.empty()) {
            float stability = 0.0f;
            
            if (p.data.size() == 1) {
                // Single values are perfectly stable
                stability = 1.0f;
            } else {
                // Calculate how consistent the pattern values are
                float sum_abs_diffs = 0.0f;
                float max_value = *std::max_element(p.data.begin(), p.data.end());
                float min_value = *std::min_element(p.data.begin(), p.data.end());
                float range = max_value - min_value;
                
                if (range < 1e-6f) {
                    // All values are essentially the same - very stable
                    stability = 0.95f;
                } else {
                    // Calculate normalized differences
                    for (size_t i = 1; i < p.data.size(); ++i) {
                        sum_abs_diffs += std::abs(p.data[i] - p.data[i-1]);
                    }
                    float avg_change = sum_abs_diffs / (p.data.size() - 1);
                    float normalized_change = avg_change / (range + 1e-6f);
                    
                    // Stability is inverse of normalized change
                    stability = std::max(0.0f, 1.0f - normalized_change);
                }
                
                // Apply coherence boost - coherent patterns are more stable
                stability = std::min(1.0f, stability + m.coherence * 0.2f);
            }
            
            m.stability = stability;
        } else {
            m.stability = 0.0f;
        }
        
        // Calculate entropy based on randomness and unpredictability
        if (!p.data.empty()) {
            // Use Shannon-like entropy calculation
            float entropy = 0.0f;
            
            if (p.data.size() > 1) {
                // Calculate differences between consecutive values
                std::vector<float> diffs;
                for (size_t i = 1; i < p.data.size(); ++i) {
                    diffs.push_back(std::abs(p.data[i] - p.data[i-1]));
                }
                
                // Calculate variance of differences (measure of unpredictability)
                float diff_mean = 0.0f;
                for (float diff : diffs) {
                    diff_mean += diff;
                }
                diff_mean /= diffs.size();
                
                float diff_variance = 0.0f;
                for (float diff : diffs) {
                    diff_variance += (diff - diff_mean) * (diff - diff_mean);
                }
                diff_variance /= diffs.size();
                
                // Entropy is higher with more variance in differences
                entropy = std::sqrt(diff_variance);
                
                // Normalize to 0-1 range and add base entropy
                entropy = std::min(1.0f, entropy + 0.1f);
            } else {
                // Single value has low entropy
                entropy = 0.1f;
            }
            
            m.entropy = entropy;
        } else {
            m.entropy = 0.5f; // Default entropy for empty patterns
        }
        current_metrics_.push_back(m);

        // Signal generation
        auto logger = sep::logging::Manager::getInstance().getLogger("pattern_engine");
        if (m.stability < signal_thresholds_.min_stability && m.entropy > signal_thresholds_.max_entropy) {
            Signal s;
            s.type = SignalType::SELL;
            s.confidence = (1.0f - m.stability) * m.entropy;
            s.pattern_id = m.pattern_id;
            current_signals_.push_back(s);
            if(logger) logger->info("SELL signal generated for pattern {}: confidence={}", s.pattern_id, s.confidence);
        } else if (m.coherence > signal_thresholds_.min_coherence && m.stability > signal_thresholds_.min_stability) {
            Signal s;
            s.type = SignalType::BUY;
            s.confidence = m.coherence * m.stability;
            s.pattern_id = m.pattern_id;
            current_signals_.push_back(s);
            if(logger) logger->info("BUY signal generated for pattern {}: confidence={}", s.pattern_id, s.confidence);
        }
    }
    return current_metrics_;
}

const std::vector<sep::compat::PatternData>& PatternMetricEngine::getPatterns() const
{
    return current_patterns_;
}

std::vector<sep::compat::PatternData> PatternMetricEngine::extractPatternsFromBytes(
    const uint8_t* data, size_t size)
{
    std::vector<sep::compat::PatternData> patterns;
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
        std::vector<float> float_vec;
        float_vec.resize(remaining_bytes / sizeof(float) + (remaining_bytes % sizeof(float) != 0));
        std::memcpy(float_vec.data(), remaining_data_ptr, remaining_bytes);

        p.data.assign(float_vec.begin(), float_vec.end());
        
        patterns.push_back(p);
    }

    return patterns;
}

void PatternMetricEngine::processBuffer([[maybe_unused]] bool is_final_chunk) {
    // Real buffer processing implementation
    std::lock_guard<std::mutex> lock(engine_mutex_);
    
    // Process streaming buffer data
    if (!stream_buffer_.empty()) {
        // Extract patterns from the buffered data
        auto extracted_patterns = extractPatternsFromBytes(
            reinterpret_cast<const uint8_t*>(stream_buffer_.data()), 
            stream_buffer_.size()
        );
        
        // Add extracted patterns to our current pattern set
        for (const auto& pattern : extracted_patterns) {
            current_patterns_.push_back(pattern);
        }
        
        // Clear the buffer after processing
        stream_buffer_.clear();
        
        // If this is the final chunk, finalize processing
        if (is_final_chunk) {
            // Run pattern evolution on all accumulated patterns
            evolvePatterns();
            
            // Compute final metrics
            computeMetrics();
            
            std::cout << "[PatternMetricEngine] Processed final chunk: " 
                      << current_patterns_.size() << " patterns, "
                      << current_metrics_.size() << " metrics computed" << std::endl;
        }
    }
}

} // namespace sep::quantum