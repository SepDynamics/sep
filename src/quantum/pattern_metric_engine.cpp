#include "quantum/pattern_metric_engine.h"

#include <algorithm>
#include <cmath>

#include "engine/internal/logging.h"
#ifdef _WIN32
#include <windows.h>
#else
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

#include "quantum/quantum_processor_cuda.h"

namespace sep::quantum
{

    PatternMetricEngine::PatternMetricEngine()
        : qfh_processor_(std::make_unique<QuantumProcessorQFH>()), use_gpu_(false)
    {
        scratch_pattern_bits_.reserve(1024);
    }

    void PatternMetricEngine::clear()
    {
        std::lock_guard<std::mutex> lock(engine_mutex_);
        current_patterns_.clear();
        current_metrics_.clear();
        pattern_cache_.clear();
        cache_dirty_ = true;
        stream_buffer_.clear();
        if (qfh_processor_)
        {
            qfh_processor_->clear();
        }
    }

    sep::SEPResult PatternMetricEngine::init([[maybe_unused]] quantum::GPUContext* ctx)
    {
        use_gpu_ = (ctx != nullptr);
        return sep::SEPResult::SUCCESS;
    }

    void PatternMetricEngine::ingestData(const uint8_t* data, size_t size)
    {
        std::lock_guard<std::mutex> lock(engine_mutex_);
        stream_buffer_.insert(stream_buffer_.end(), data, data + size);
    }

    void PatternMetricEngine::ingestData(std::istream& stream)
    {
        std::vector<uint8_t> buffer(4096);
        while (stream.read(reinterpret_cast<char*>(buffer.data()), buffer.size()))
        {
            ingestData(buffer.data(), stream.gcount());
        }
        ingestData(buffer.data(), stream.gcount());
    }

    void PatternMetricEngine::ingestFile(const std::string& filepath)
    {
        std::ifstream file(filepath, std::ios::binary);
        if (file)
        {
            ingestData(file);
        }
    }

    void PatternMetricEngine::ingestMappedFile(const std::string& filepath)
    {
#ifdef _WIN32
        HANDLE file = CreateFileA(filepath.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr,
                                  OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
        if (file == INVALID_HANDLE_VALUE)
        {
            return;
        }

        LARGE_INTEGER size{};
        if (!GetFileSizeEx(file, &size))
        {
            CloseHandle(file);
            return;
        }

        HANDLE mapping = CreateFileMappingA(file, nullptr, PAGE_READONLY, 0, 0, nullptr);
        if (!mapping)
        {
            CloseHandle(file);
            return;
        }

        void* data = MapViewOfFile(mapping, FILE_MAP_READ, 0, 0, 0);
        if (data)
        {
            ingestData(static_cast<uint8_t*>(data), static_cast<size_t>(size.QuadPart));
            UnmapViewOfFile(data);
        }

        CloseHandle(mapping);
        CloseHandle(file);
#else
        int fd = open(filepath.c_str(), O_RDONLY);
        if (fd == -1)
        {
            return;
        }

        struct stat sb{};
        if (fstat(fd, &sb) == -1)
        {
            close(fd);
            return;
        }

        void* map = mmap(nullptr, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
        if (map == MAP_FAILED)
        {
            close(fd);
            return;
        }

        ingestData(static_cast<uint8_t*>(map), static_cast<size_t>(sb.st_size));

        munmap(map, sb.st_size);
        close(fd);
#endif
    }

    void PatternMetricEngine::evolvePatterns()
    {
        std::lock_guard<std::mutex> lock(engine_mutex_);
        if (stream_buffer_.empty())
        {
            return;
        }

        // Use the QFH processor to analyze the raw data
        if (qfh_processor_)
        {
            // Convert bytes to uint32_t for QFH processing using preallocated buffer
            scratch_pattern_bits_.clear();
            scratch_pattern_bits_.reserve((stream_buffer_.size() + 3) / 4);

            for (size_t i = 0; i < stream_buffer_.size(); i += 4)
            {
                uint32_t value = 0;
                for (size_t j = 0; j < 4 && (i + j) < stream_buffer_.size(); ++j)
                {
                    value |= (static_cast<uint32_t>(stream_buffer_[i + j]) << (j * 8));
                }
                scratch_pattern_bits_.push_back(value);
            }

            // This will trigger the QFH analysis which logs "analyze: events size: X"
            qfh_processor_->processPatternBits(scratch_pattern_bits_);
        }

        // Process the new data in the buffer, append patterns, and clear the buffer.
        auto new_patterns = extractPatternsFromBytes(stream_buffer_.data(), stream_buffer_.size());
        for (const auto& p : new_patterns)
        {
            current_patterns_.push_back(p);
            if (current_patterns_.size() > max_history_size_)
            {
                current_patterns_.pop_front();
            }
        }
        cache_dirty_ = true;
        stream_buffer_.clear();
    }

    void PatternMetricEngine::addPattern(const sep::compat::PatternData& pattern)
    {
        std::lock_guard<std::mutex> lock(engine_mutex_);
        current_patterns_.push_back(pattern);
        if (current_patterns_.size() > max_history_size_)
        {
            current_patterns_.pop_front();
        }
        sep::logging::logPatternDetected(pattern.id, std::chrono::system_clock::now());
        cache_dirty_ = true;
    }

    sep::compat::PatternData PatternMetricEngine::mutatePattern(
        const sep::compat::PatternData& parent)
    {
        sep::compat::PatternData mutated = parent;
        std::string parent_id(parent.id);
        std::string new_id = parent_id + "_child";
        std::strncpy(mutated.id, new_id.c_str(), sizeof(mutated.id) - 1);
        mutated.id[sizeof(mutated.id) - 1] = '\0';
        mutated.generation++;
        if (!mutated.data.empty())
        {
            mutated.data[0] += 0.1f;
        }
        return mutated;
    }

    void PatternMetricEngine::setSignalThresholds(const SignalThresholds& thresholds)
    {
        std::lock_guard<std::mutex> lock(engine_mutex_);
        signal_thresholds_ = thresholds;
        if (auto logger = sep::logging::Manager::getInstance().getLogger("pattern_engine"))
        {
            logger->info(
                "Signal thresholds updated: buy_coh={} buy_stab={} buy_entropy={} sell_stab={} "
                "sell_entropy={}",
                signal_thresholds_.buy_min_coherence, signal_thresholds_.buy_min_stability,
                signal_thresholds_.buy_max_entropy, signal_thresholds_.sell_max_stability,
                signal_thresholds_.sell_min_entropy);
        }
    }

    void PatternMetricEngine::setBuyThresholds(float min_coherence, float min_stability,
                                               float max_entropy)
    {
        std::lock_guard<std::mutex> lock(engine_mutex_);
        signal_thresholds_.buy_min_coherence = min_coherence;
        signal_thresholds_.buy_min_stability = min_stability;
        signal_thresholds_.buy_max_entropy = max_entropy;
    }

    void PatternMetricEngine::setSellThresholds(float max_stability, float min_entropy)
    {
        std::lock_guard<std::mutex> lock(engine_mutex_);
        signal_thresholds_.sell_max_stability = max_stability;
        signal_thresholds_.sell_min_entropy = min_entropy;
    }

    const std::vector<Signal>& PatternMetricEngine::getSignals() const { return current_signals_; }

    const std::vector<PatternMetrics>& PatternMetricEngine::computeMetrics() const
    {
        std::lock_guard<std::mutex> lock(engine_mutex_);

        current_metrics_.clear();
        current_metrics_.reserve(current_patterns_.size());

        scratch_diffs_.clear();

        for (const auto& p : current_patterns_)
        {
            PatternMetrics m;
            std::strncpy(m.pattern_id, p.id, sizeof(m.pattern_id) - 1);
            m.pattern_id[sizeof(m.pattern_id) - 1] = '\0';
            m.length = p.data.size();

            float sum_squares = 0.0f;

            if (!p.data.empty())
            {
                // Calculate coherence based on pattern self-similarity and consistency
                float mean = 0.0f;

                // Calculate mean
                for (float val : p.data)
                {
                    mean += val;
                }
                mean /= p.data.size();

                // Calculate variance and coherence
                float variance = 0.0f;
            for (float val : p.data)
            {
                float diff = val - mean;
                variance += diff * diff;
                sum_squares += val * val;
            }
                variance /= p.data.size();

                // Coherence is high when variance is low relative to signal strength
                // Use coefficient of variation (inverse) as coherence measure
                if (std::abs(mean) > 1e-6f)
                {
                    float cv = std::sqrt(variance) / std::abs(mean);
                    m.coherence = std::max(0.0f, std::min(1.0f, 1.0f / (1.0f + cv)));
                }
                else
                {
                    // For zero-mean patterns, use energy concentration
                    float energy = sum_squares / p.data.size();
                    m.coherence =
                        std::max(0.0f, std::min(1.0f, energy / (energy + variance + 1e-6f)));
                }

                // Boost coherence if pattern has clear structure
                if (p.data.size() >= 4)
                {
                    // Check for repeating patterns or structure
                    float structure_bonus = 0.0f;
                    for (size_t i = 0; i < p.data.size() - 1; ++i)
                    {
                        if (std::abs(p.data[i] - p.data[i + 1]) < 0.1f)
                        {
                            structure_bonus += 0.05f;
                        }
                    }
                    m.coherence = std::min(1.0f, m.coherence + structure_bonus);
                }
            }
            else
            {
                m.coherence = 0.0f;
            }

            m.energy = sum_squares;

            if (!p.data.empty())
            {
                m.stability = calculateStability(p.data, m.coherence);
            }
            else
            {
                m.stability = 0.0f;
            }

            if (!p.data.empty())
            {
                m.entropy = calculateEntropy(p.data);
            }
            else
            {
                m.entropy = 0.5f;
            }
            current_metrics_.push_back(m);
        }

        // Cannot call generateSignals() from const method
        // generateSignals();
        return current_metrics_;
    }

    AggregateMetrics PatternMetricEngine::computeAggregateMetrics() const
    {
        AggregateMetrics agg;
        const auto& metrics = computeMetrics();
        if (metrics.empty())
        {
            return agg;
        }

        for (const auto& m : metrics)
        {
            agg.average_coherence += m.coherence;
            agg.average_stability += m.stability;
            agg.average_entropy += m.entropy;
            agg.average_energy += m.energy;
        }

        float n = static_cast<float>(metrics.size());
        agg.average_coherence /= n;
        agg.average_stability /= n;
        agg.average_entropy /= n;
        agg.average_energy /= n;

        return agg;
    }

    const std::vector<sep::compat::PatternData>& PatternMetricEngine::getPatterns() const
    {
        if (cache_dirty_)
        {
            pattern_cache_.assign(current_patterns_.begin(), current_patterns_.end());
            cache_dirty_ = false;
        }
        return pattern_cache_;
    }

    std::vector<sep::compat::PatternData> PatternMetricEngine::extractPatternsFromBytes(
        const uint8_t* data, size_t size)
    {
        std::vector<sep::compat::PatternData> patterns;
        const size_t float_size = sizeof(float);
        const size_t chunk_size_floats = 16;
        const size_t chunk_size_bytes = chunk_size_floats * float_size;

        if (size == 0)
        {
            return patterns;
        }

        size_t num_patterns = size / chunk_size_bytes;
        for (size_t i = 0; i < num_patterns; ++i)
        {
            sep::compat::PatternData p;
            std::string id_str = "pattern_" + std::to_string(i);
            std::strncpy(p.id, id_str.c_str(), sizeof(p.id) - 1);
            p.id[sizeof(p.id) - 1] = '\0';

            const float* float_data = reinterpret_cast<const float*>(data + (i * chunk_size_bytes));
            p.data.assign(float_data, float_data + chunk_size_floats);

            patterns.push_back(p);
        }

        size_t remaining_bytes = size % chunk_size_bytes;
        if (remaining_bytes > 0)
        {
            sep::compat::PatternData p;
            std::string id_str = "pattern_" + std::to_string(num_patterns);
            std::strncpy(p.id, id_str.c_str(), sizeof(p.id) - 1);
            p.id[sizeof(p.id) - 1] = '\0';

            const uint8_t* remaining_data_ptr = data + (num_patterns * chunk_size_bytes);
            std::vector<float> float_vec;
            float_vec.resize(remaining_bytes / sizeof(float) +
                             (remaining_bytes % sizeof(float) != 0));
            std::memcpy(float_vec.data(), remaining_data_ptr, remaining_bytes);

            p.data.assign(float_vec.begin(), float_vec.end());

            patterns.push_back(p);
        }

        return patterns;
    }

    void PatternMetricEngine::generateSignals()
    {
        current_signals_.clear();
        for (const auto& m : current_metrics_)
        {
            SignalType type = evaluateSignal(m);
            Signal s;
            s.type = type;
            s.pattern_id = m.pattern_id;

            if (type != SignalType::HOLD)
            {
                if (type == SignalType::BUY)
                {
                    s.confidence = m.coherence * m.stability;
                }
                else
                {
                    s.confidence = (1.0f - m.stability) * m.entropy;
                }
                current_signals_.push_back(s);
                auto now = std::chrono::system_clock::now();
                sep::logging::logPatternDetected(s.pattern_id, now);
                sep::logging::logSignalDetected(s.pattern_id, s.type, now);
            }
        }
    }

    // Evaluate signal based on threshold rules defined in docs/TODO.md
    SignalType PatternMetricEngine::evaluateSignal(const PatternMetrics& m) const
    {
        const auto& th = signal_thresholds_;

        if (m.stability < th.sell_max_stability && m.entropy > th.sell_min_entropy)
        {
            return SignalType::SELL;
        }

        if (m.coherence > th.buy_min_coherence && m.stability > th.buy_min_stability &&
            m.entropy < th.buy_max_entropy)
        {
            return SignalType::BUY;
        }

        return SignalType::HOLD;
    }

    void PatternMetricEngine::processBuffer([[maybe_unused]] bool is_final_chunk)
    {
        // Real buffer processing implementation
        std::lock_guard<std::mutex> lock(engine_mutex_);

        // Process streaming buffer data
        if (!stream_buffer_.empty())
        {
            // Extract patterns from the buffered data
            auto extracted_patterns = extractPatternsFromBytes(
                reinterpret_cast<const uint8_t*>(stream_buffer_.data()), stream_buffer_.size());

            // Add extracted patterns to our current pattern set with history limit
            for (const auto& pattern : extracted_patterns)
            {
                current_patterns_.push_back(pattern);
                sep::logging::logPatternDetected(pattern.id, std::chrono::system_clock::now());
                if (current_patterns_.size() > max_history_size_)
                {
                    current_patterns_.pop_front();
                }
            }
            cache_dirty_ = true;

            // Clear the buffer after processing
            stream_buffer_.clear();

            // If this is the final chunk, finalize processing
            if (is_final_chunk)
            {
                // Run pattern evolution on all accumulated patterns
                evolvePatterns();

                // Compute final metrics
                computeMetrics();

                std::cout << "[PatternMetricEngine] Processed final chunk: "
                          << current_patterns_.size() << " patterns, " << current_metrics_.size()
                          << " metrics computed" << std::endl;
            }
        }
    }

}  // namespace sep::quantum