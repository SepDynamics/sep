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
    shim::string host{"http://127.0.0.1:11434"};
    shim::string model{"llama2"};
    std::size_t batch_size{1};
    std::size_t context_window{512};
    GPUConfig gpu{};
};

struct GenerateRequest {
    shim::string model;
    shim::string prompt;
    shim::string system;
    bool stream{false};
};

struct GenerateResponse {
    shim::string response;
    bool done{false};
    shim::string model;
};

struct EmbeddingRequest {
    shim::string model;
    shim::string prompt;
};

struct EmbeddingResponse {
    shim::vector<float> embedding;
};

} // namespace sep::ollama
