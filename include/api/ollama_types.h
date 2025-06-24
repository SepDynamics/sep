#pragma once
#include <string>
#include <vector>

namespace sep::ollama {

struct GPUConfig {
    bool enabled{false};
    float memory_fraction{0.0f};
};

struct OllamaConfig {
    bool enabled{false};
    std::string host{"http://127.0.0.1:11434"};
    std::string model{"llama2"};
    std::size_t batch_size{1};
    std::size_t context_window{512};
    GPUConfig gpu{};
};

struct GenerateRequest {
    std::string model;
    std::string prompt;
    std::string system;
    bool stream{false};
};

struct GenerateResponse {
    std::string response;
    bool done{false};
    std::string model;
};

struct EmbeddingRequest {
    std::string model;
    std::string prompt;
};

struct EmbeddingResponse {
    std::vector<float> embedding;
};

} // namespace sep::ollama
