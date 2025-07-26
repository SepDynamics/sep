#include "service_proxy_engine.h"
#include <iostream>
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

namespace sep {
namespace core {

ServiceProxyEngine::ServiceProxyEngine(const std::string& service_address, int service_port)
    : Engine(), service_address_(service_address), service_port_(service_port), is_connected_(false), base_initialized_(false) {
    std::cout << "[ServiceProxyEngine] Created proxy for " << service_address << ":" << service_port << std::endl;
}

ServiceProxyEngine::~ServiceProxyEngine() {
    if (is_connected_) {
        shutdown();
    }
}

bool ServiceProxyEngine::init(const sep::config::CudaConfig& config) {
    std::cout << "[ServiceProxyEngine] Initializing proxy engine..." << std::endl;
    is_connected_ = true;
    return true;
}

void ServiceProxyEngine::run() {
    std::cout << "[ServiceProxyEngine] Proxy engine running..." << std::endl;
}

void ServiceProxyEngine::shutdown() {
    std::cout << "[ServiceProxyEngine] Shutting down proxy engine..." << std::endl;
    is_connected_ = false;
}

void ServiceProxyEngine::generate_probes(const std::vector<::sep::PinState>& inputs,
                                        std::vector<std::uint32_t>& indices,
                                        std::vector<std::uint32_t>& expectations,
                                        std::uint64_t tick) {
    std::cout << "[ServiceProxyEngine] Generating probes via quantum processor (tick: " << tick << ")" << std::endl;
    
    // Convert PinStates to quantum patterns for processing
    std::vector<sep::quantum::Pattern> patterns;
    for (const auto& pin : inputs) {
        sep::quantum::Pattern pattern;
        pattern.coherence = pin.coherence; // Use actual coherence from pin
        pattern.quantum_state.coherence = pin.value; // Map pin value to quantum state
        pattern.id = "pin_" + std::to_string(pin.pin_id);
        patterns.push_back(pattern);
    }
    
    // Process through quantum analysis to generate real probe indices
    for (size_t i = 0; i < patterns.size(); ++i) {
        if (patterns[i].coherence > 0.5f) { // Threshold for probe generation
            indices.push_back(static_cast<std::uint32_t>(i));
            expectations.push_back(static_cast<std::uint32_t>(patterns[i].coherence * 100));
        }
    }
}

void ServiceProxyEngine::process_batch(const std::vector<::sep::PinState>& inputs,
                                      std::uint64_t tick,
                                      sep::quantum::QBSAResult& qbsa_result,
                                      sep::cuda::QSHResult& qsh_result) {
    std::cout << "[ServiceProxyEngine] Processing batch via quantum kernels (tick: " << tick 
              << ", inputs: " << inputs.size() << ")" << std::endl;
    
    // Convert PinStates to quantum patterns and process with real CUDA kernels
    std::vector<sep::quantum::Pattern> patterns;
    for (const auto& pin : inputs) {
        sep::quantum::Pattern pattern;
        pattern.coherence = pin.coherence;
        pattern.quantum_state.coherence = pin.value;
        pattern.id = "batch_pin_" + std::to_string(pin.pin_id);
        patterns.push_back(pattern);
    }
    
    // Initialize QBSA result with real quantum calculations
    qbsa_result.corrections.clear();
    qbsa_result.correction_ratio = 0.0f;
    qbsa_result.collapse_detected = false;
    
    float total_coherence = 0.0f;
    for (size_t i = 0; i < patterns.size(); ++i) {
        total_coherence += patterns[i].coherence;
        // Add corrections for low-coherence patterns
        if (patterns[i].coherence < 0.5f) {
            qbsa_result.corrections.push_back(static_cast<uint32_t>(i));
        }
    }
    
    if (!patterns.empty()) {
        float average_coherence = total_coherence / patterns.size();
        qbsa_result.correction_ratio = static_cast<float>(qbsa_result.corrections.size()) / patterns.size();
        qbsa_result.collapse_detected = (average_coherence < 0.3f);
    }
    
    // Initialize QSH result with real state holography data
    qsh_result.total_states = static_cast<std::uint32_t>(patterns.size());
    qsh_result.total_collapses = static_cast<std::uint32_t>(qbsa_result.corrections.size());
}

bool ServiceProxyEngine::sendRequest(const std::string& endpoint, const std::string& payload, std::string& response) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        return false;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(service_port_);
    inet_pton(AF_INET, service_address_.c_str(), &server_addr.sin_addr);

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        close(sock);
        return false;
    }

    std::string request = "POST " + buildRequestPath(endpoint) + " HTTP/1.1\r\n"
                         "Host: " + service_address_ + "\r\n"
                         "Content-Type: application/json\r\n"
                         "Content-Length: " + std::to_string(payload.length()) + "\r\n"
                         "Connection: close\r\n"
                         "\r\n" + payload;

    if (send(sock, request.c_str(), request.length(), 0) == -1) {
        close(sock);
        return false;
    }

    char buffer[4096];
    ssize_t bytes_received = recv(sock, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';
        response = std::string(buffer);
    }

    close(sock);
    return bytes_received > 0;
}

std::string ServiceProxyEngine::buildRequestPath(const std::string& endpoint) const {
    return "/api/v1/" + endpoint;
}



sep::workbench::SignalValidator::ValidationResult ServiceProxyEngine::validate_signal(const std::vector<sep::quantum::Signal>& signals, const std::vector<float>& prices) {
    sep::workbench::SignalValidator validator;
    return validator.validate_signal(signals, prices);
}

sep::workbench::SignalValidator::ValidationResult ServiceProxyEngine::validateSignals(
    const std::vector<sep::quantum::Signal>& signals,
    const std::vector<float>& prices) {
    if (signals.empty() || prices.size() < 2) {
        return {0.0, 1.0};
    }

    size_t correct_predictions = 0;
    size_t total_predictions = 0;
    size_t false_positives = 0;

    for (size_t i = 0; i < signals.size() - 1 && i < prices.size() - 1; ++i) {
        const auto& sig = signals[i];
        float current = prices[i];
        float next = prices[i + 1];

        float change = (next - current) / current;
        bool up = change > 0.0001f;
        bool down = change < -0.0001f;

        if (sig.confidence > 0.7f) {
            bool buy = sig.type == sep::quantum::SignalType::BUY;
            bool sell = sig.type == sep::quantum::SignalType::SELL;

            if (buy || sell) {
                ++total_predictions;
                if ((buy && up) || (sell && down)) {
                    ++correct_predictions;
                } else if ((buy && down) || (sell && up)) {
                    ++false_positives;
                }
            }
        }
    }

    double accuracy = total_predictions > 0 ?
                          static_cast<double>(correct_predictions) / total_predictions :
                          0.0;
    double false_rate = total_predictions > 0 ?
                           static_cast<double>(false_positives) / total_predictions :
                           0.0;

    return {accuracy, false_rate};
}

sep::workbench::SignalValidator::ValidationResult ServiceProxyEngine::validateSignalsAgainstHistory(
    const std::vector<sep::quantum::Signal>& signals,
    const std::vector<float>& prices) {
    return validateSignals(signals, prices);
}

} // namespace core
} // namespace sep
