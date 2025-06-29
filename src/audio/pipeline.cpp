#include "audio/pipeline.h"
#include "compat/math_common.h"
#include <complex>
#include <algorithm>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <queue>
#include <vector>

namespace sep {
namespace audio {
namespace {
} // anonymous namespace
AudioPipeline::AudioPipeline(size_t sample_rate, size_t channels)
    : m_sample_rate(sample_rate),
      m_channels(channels),
      m_frame_size(2048) {}

void AudioPipeline::setPatternQueue(std::queue<glm::vec3>* queue) {
    m_pattern_queue = queue;
}

void AudioPipeline::processAudioFrame(const std::vector<float>& samples) {
    // Create audio frame
    AudioFrame frame;
    frame.samples = samples;
    frame.sample_rate = m_sample_rate;
    frame.channels = m_channels;
    frame.timestamp = std::chrono::system_clock::now();

    // Queue frame for processing
    m_frame_queue.push(frame);

    // Process frames if enough are queued
    while (m_frame_queue.size() >= m_frame_size) {
        processQueuedFrames();
    }
}

std::vector<glm::vec3> AudioPipeline::getPatterns() {
    std::vector<glm::vec3> patterns;

    // Convert processed spectral data to patterns
    while (!m_spectral_queue.empty()) {
        auto& spectral = m_spectral_queue.front();
        glm::vec3 pattern = convertToPattern(spectral);
        patterns.push_back(pattern);
        if (m_pattern_queue) {
            m_pattern_queue->push(pattern);
        }
        m_spectral_queue.pop();
    }

    return patterns;
}

void AudioPipeline::processQueuedFrames() {
    // Collect samples for FFT
    std::vector<float> frame_buffer;
    for (size_t i = 0; i < m_frame_size; ++i) {
        const auto& frame = m_frame_queue.front();
        frame_buffer.insert(frame_buffer.end(),
                          frame.samples.begin(),
                          frame.samples.end());
        m_frame_queue.pop();
    }

    // Apply window function
    applyHannWindow(frame_buffer);

    // Perform FFT
    auto spectral = performFFT(frame_buffer);

    // Calculate spectral features
    calculateSpectralFeatures(spectral);

    // Queue processed data
    m_spectral_queue.push(spectral);
}

void AudioPipeline::applyHannWindow(std::vector<float>& samples) {
    for (size_t i = 0; i < samples.size(); ++i) {
        float angle = 2.0f * glm::pi<float>() * static_cast<float>(i) / // Fix: Cast i to float // Fix: Added comment
                      static_cast<float>(samples.size());
        float window = 0.5f * (1.0f - std::cos(angle));
        samples[i] *= window;
    }
}

SpectralData AudioPipeline::performFFT(const std::vector<float>& samples) {
    SpectralData spectral;

    // Prepare FFT input
    std::vector<std::complex<float>> fft_input(samples.size());
    for (size_t i = 0; i < samples.size(); ++i) {
        fft_input[i] = std::complex<float>(samples[i], 0.0f);
    }

    // Perform FFT (simplified for demo)
    spectral.fft = fft_input;  // Would use actual FFT implementation

    // Calculate magnitudes and phases
    spectral.magnitudes.resize(samples.size() / 2 + 1);
    spectral.phases.resize(samples.size() / 2 + 1);

    for (size_t i = 0; i < spectral.magnitudes.size(); ++i) {
        auto& bin = spectral.fft[i];
        spectral.magnitudes[i] = std::abs(bin);
        spectral.phases[i] = std::arg(bin);
    }

    return spectral;
}

void AudioPipeline::calculateSpectralFeatures(SpectralData& spectral) {
    // Calculate fundamental frequency
    spectral.fundamental_freq = findFundamentalFrequency(spectral.magnitudes);

    // Calculate spectral centroid
    spectral.spectral_centroid = calculateSpectralCentroid(spectral.magnitudes);
}

float AudioPipeline::findFundamentalFrequency(const std::vector<float>& magnitudes) {
    // Find peak magnitude
    auto peak_it = std::max_element(magnitudes.begin(), magnitudes.end());
    size_t peak_bin = std::distance(magnitudes.begin(), peak_it);

    // Convert bin to frequency
    return static_cast<float>(peak_bin) * m_sample_rate / m_frame_size;
}

float AudioPipeline::calculateSpectralCentroid(const std::vector<float>& magnitudes) {
    float weighted_sum = 0.0f;
    float magnitude_sum = 0.0f;

    for (size_t i = 0; i < magnitudes.size(); ++i) {
        float freq = static_cast<float>(i) * m_sample_rate / m_frame_size;
        weighted_sum += freq * magnitudes[i];
        magnitude_sum += magnitudes[i];
    }

    return weighted_sum / magnitude_sum;
}

glm::vec3 AudioPipeline::convertToPattern(const SpectralData& spectral) {
    // Convert spectral features to pattern vector
    return glm::vec3(
        spectral.fundamental_freq / (m_sample_rate / 2.0f),  // Normalized frequency
        spectral.spectral_centroid / (m_sample_rate / 2.0f), // Normalized centroid
        calculateSpectralFlux(spectral)                      // Spectral flux
    );
}

float AudioPipeline::calculateSpectralFlux(const SpectralData& spectral) {
    static std::vector<float> prev_magnitudes;

    float flux = 0.0f;
    if (!prev_magnitudes.empty()) {
        for (size_t i = 0; i < spectral.magnitudes.size(); ++i) {
            float diff = spectral.magnitudes[i] - prev_magnitudes[i];
            flux += diff > 0.0f ? diff : 0.0f;  // Only positive changes
        }
    }

    prev_magnitudes = spectral.magnitudes;
    return flux;
}

} // namespace audio
} // namespace sep
