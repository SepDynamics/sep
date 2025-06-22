#pragma once

#include <queue>
#include <vector>
#include <complex>
#include <chrono>
#include <glm/glm.hpp>

namespace sep {
namespace audio {

struct AudioFrame {
    std::vector<float> samples;
    size_t sample_rate;
    size_t channels;
    std::chrono::system_clock::time_point timestamp;
};

struct SpectralData {
    std::vector<std::complex<float>> fft;
    std::vector<float> magnitudes;
    std::vector<float> phases;
    float fundamental_freq;
    float spectral_centroid;
};

class AudioPipeline {
 public:
  AudioPipeline(size_t sample_rate = 44100, size_t channels = 2);

  void processAudioFrame(const std::vector<float>& samples);
  std::vector<glm::vec3> getPatterns();

  // Destination for processed pattern vectors
  void setPatternQueue(std::queue<glm::vec3>* queue);

 private:
  AudioPipeline(const AudioPipeline&) = delete;
  AudioPipeline& operator=(const AudioPipeline&) = delete;

  // Member variables
  size_t m_sample_rate;
  size_t m_channels;
  size_t m_frame_size;
  std::queue<AudioFrame> m_frame_queue;
  std::queue<SpectralData> m_spectral_queue;
  std::queue<glm::vec3>* m_pattern_queue{nullptr};

  // Private methods
  void processQueuedFrames();
  void applyHannWindow(std::vector<float>& samples);
  SpectralData performFFT(const std::vector<float>& samples);
  void calculateSpectralFeatures(SpectralData& spectral);
  float findFundamentalFrequency(const std::vector<float>& magnitudes);
  float calculateSpectralCentroid(const std::vector<float>& magnitudes);
  glm::vec3 convertToPattern(const SpectralData& spectral);
  float calculateSpectralFlux(const SpectralData& spectral);
};

}  // namespace audio
}  // namespace sep

