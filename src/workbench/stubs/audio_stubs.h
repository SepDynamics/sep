#pragma once

#include <vector>
#include <cstdint>

namespace sep::audio {

struct AudioSample {
    float amplitude{0.0f};
    float frequency{440.0f};
    uint64_t timestamp{0};
};

class AudioCapture {
public:
    AudioCapture() = default;
    ~AudioCapture() = default;
    
    bool initialize() { return true; }
    void shutdown() {}
    
    bool isRecording() const { return false; }
    void startRecording() {}
    void stopRecording() {}
    
    std::vector<AudioSample> getLatestSamples() { return {}; }
    std::vector<float> getFrequencySpectrum() { return std::vector<float>(256, 0.0f); }
    
    float getVolume() const { return 0.0f; }
    float getDominantFrequency() const { return 440.0f; }
};

inline std::unique_ptr<AudioCapture> createAudioCapture() {
    return std::make_unique<AudioCapture>();
}

}
