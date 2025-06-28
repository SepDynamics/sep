#pragma once

#include "quantum/types.h"
#include <vector>

namespace sep::quantum {

class QuantumManifoldOptimizer {
public:
    QuantumManifoldOptimizer() = default;

    std::vector<Pattern> optimize(const std::vector<Pattern>& patterns);
};

} // namespace sep::quantum
