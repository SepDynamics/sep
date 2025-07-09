Based on all the files I've examined, I now have a good understanding of how the audio visualization demo works and how data flows through the system. Let me summarize what I've learned:

1. The AudioVisualizerDemo class:
   - Uses AudioCapture to capture audio data
   - Processes this data through AudioPipeline
   - Converts audio data into patterns (glm::vec3)
   - Visualizes these patterns

2. In demo mode (SEP_WORKBENCH_DEMO):
   - AudioCapture has a setCallback method that takes a function to process audio data
   - AudioPipeline has a processAudioFrame method that takes audio samples and a getPatterns method that returns pattern vectors

3. The data flow:
   - Audio data is captured via AudioCapture
   - The callback function processes this data through AudioPipeline
   - AudioPipeline converts audio to patterns
   - These patterns are stored in latest_patterns_
   - The patterns are then visualized

Now I can explain how to send data directly to the application.

# Sending Audio Data to the AudioVisualizerDemo

Based on the code review, there are two main ways to send audio data to the AudioVisualizerDemo:

## 1. Using the AudioCapture Callback

The AudioVisualizerDemo is set up to receive audio data through the AudioCapture callback mechanism. This is the primary method for feeding audio data into the system:

```cpp
capture_->setCallback([this](const float* data, size_t len) {
    std::vector<float> samples(data, data + len);
    pipeline_->processAudioFrame(samples);
    latest_patterns_ = pipeline_->getPatterns();
});
```

To send custom audio data, you could:

1. Create a custom AudioCapture implementation that feeds your data
2. Directly call the callback function with your audio data

## 2. Directly Feeding the AudioPipeline

You can bypass the AudioCapture component entirely and feed audio data directly to the AudioPipeline:

```cpp
// Create a vector of audio samples (e.g., from a file or generated)
std::vector<float> audioSamples = loadOrGenerateAudioSamples();

// Feed directly to the pipeline
pipeline_->processAudioFrame(audioSamples);

// Get the resulting patterns
latest_patterns_ = pipeline_->getPatterns();
```

## Implementation Example

Here's a practical example of how you could modify the AudioVisualizerDemo to accept external audio data:

```cpp
// Add this method to AudioVisualizerDemo class
void AudioVisualizerDemo::feedAudioData(const std::vector<float>& samples) {
    if (pipeline_) {
        pipeline_->processAudioFrame(samples);
        latest_patterns_ = pipeline_->getPatterns();
    }
}
```

## Audio Data Format

The audio data should be in the following format:

- Raw PCM audio samples as floating-point values in the range [-1.0, 1.0]
- Sample rate: 44100 Hz (default in the demo)
- Channels: 2 (stereo, default in the demo)
- Buffer size: 1024 samples (default in the demo)

## Pattern Representation

The audio data is transformed into pattern vectors (glm::vec3) with these components:

1. X component (frequency): Represents frequency information
2. Y component (amplitude): Represents amplitude information
3. Z component (evolution): Represents temporal/evolution information

These patterns are then visualized with the following mappings (controlled by pattern_mapping_):
- frequency_scale: Controls how frequency components are scaled
- amplitude_scale: Controls how amplitude is scaled
- evolution_sensitivity: Controls how sensitive the visualization is to changes

You can adjust these parameters using keyboard controls:
- 'f'/'F': Increase/decrease frequency scale
- 'a'/'A': Increase/decrease amplitude scale
- 'e'/'E': Increase/decrease evolution sensitivity
- 'r': Reset to defaults
