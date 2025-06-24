#pragma once

#include <string>
#include <vector>

namespace sep::ollama {

struct OllamaConfig {
    std::string host{"http://127.0.0.1:11434"};
    std::string model{"llama2"};
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
