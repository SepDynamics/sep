#pragma once

#include "engine/engine.h"
#include "engine/standard_includes.h"
#include "quantum/qbsa.h"
#include "quantum/types.h"
#include <string>
#include <memory>
#include <vector>
#include <cstdint>

namespace sep {
namespace core {

/**
 * @brief Proxy engine that forwards operations to a remote SEP service
 * Since Engine methods aren't virtual, we compose rather than inherit
 */
class ServiceProxyEngine : public Engine {
public:
    ServiceProxyEngine(const std::string& service_address, int service_port);
    ~ServiceProxyEngine();

    // These hide the base Engine methods and forward to service
    bool init(const sep::config::CudaConfig& config);
    void run();
    void shutdown();

    void generate_probes(const std::vector<::sep::PinState>& inputs,
                        std::vector<std::uint32_t>& indices,
                        std::vector<std::uint32_t>& expectations,
                        std::uint64_t tick);

    void process_batch(const std::vector<::sep::PinState>& inputs,
                      std::uint64_t tick,
                      sep::quantum::QBSAResult& qbsa_result,
                      sep::cuda::QSHResult& qsh_result);

    bool isConnected() const { return is_connected_; }

private:
    std::string service_address_;
    int service_port_;
    bool is_connected_;
    bool base_initialized_;

    // Helper methods for HTTP communication
    bool sendRequest(const std::string& endpoint, const std::string& payload, std::string& response);
    std::string buildRequestPath(const std::string& endpoint) const;
};

} // namespace core
} // namespace sep
