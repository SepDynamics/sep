#include "quantum/quantum_manifold_optimizer.h"
#include <spdlog/spdlog.h>

namespace sep::quantum {

std::vector<Pattern> QuantumManifoldOptimizer::optimize(const std::vector<Pattern>& patterns) {
    spdlog::info("QuantumManifoldOptimizer::optimize called with {} patterns", patterns.size());
    return patterns;
}

} // namespace sep::quantum
