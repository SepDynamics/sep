#include "audio/pipeline.h"
#include "compat/math_common.h"
#include <complex>
#include <algorithm>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <compat/cufft.h>
#include <fftw3.h>
#include <queue>
#include <vector>
#include "compat/cufft.h"
#ifdef SEP_USE_FFTW
#include <fftw3.h>
#endif

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
    const size_t N = samples.size();

#if SEP_CUDA_AVAILABLE
    cufftHandle plan;
    cufftPlan1d(&plan, static_cast<int>(N), CUFFT_R2C, 1);
    std::vector<cufftReal> input(samples.begin(), samples.end());
    std::vector<cufftComplex> output(N / 2 + 1);
    cufftExecR2C(plan, input.data(), output.data());
    cufftDestroy(plan);

    spectral.fft.resize(output.size());
    for (size_t i = 0; i < output.size(); ++i) {
        spectral.fft[i] = std::complex<float>(output[i].x, output[i].y);
    }
#elif defined(SEP_USE_FFTW)
    fftwf_complex* out = reinterpret_cast<fftwf_complex*>(
        fftwf_malloc(sizeof(fftwf_complex) * (N / 2 + 1)));
    fftwf_plan plan = fftwf_plan_dft_r2c_1d(static_cast<int>(N),
                                           const_cast<float*>(samples.data()),
                                           out, FFTW_ESTIMATE);
    fftwf_execute(plan);
    fftwf_destroy_plan(plan);

    spectral.fft.resize(N / 2 + 1);
    for (size_t i = 0; i < spectral.fft.size(); ++i) {
        spectral.fft[i] = std::complex<float>(out[i][0], out[i][1]);
    }
    fftwf_free(out);
#else
    spectral.fft.resize(N);
    for (size_t k = 0; k < N; ++k) {
        std::complex<float> sum(0.0f, 0.0f);
        for (size_t n = 0; n < N; ++n) {
            float angle = -2.0f * glm::pi<float>() * static_cast<float>(n * k) /
                          static_cast<float>(N);
            sum += std::complex<float>(samples[n] * std::cos(angle),
                                      samples[n] * std::sin(angle));
        }
        spectral.fft[k] = sum;
    }
#endif

    spectral.magnitudes.resize(spectral.fft.size());
    spectral.phases.resize(spectral.fft.size());
    for (size_t i = 0; i < spectral.fft.size(); ++i) {
        spectral.magnitudes[i] = std::abs(spectral.fft[i]);
        spectral.phases[i] = std::arg(spectral.fft[i]);
    }

    fftwf_destroy_plan(plan);
    fftwf_free(out);
#endif

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
