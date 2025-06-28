#ifndef SEP_QUANTUM_MANIFOLD_OPTIMIZER_H
#define SEP_QUANTUM_MANIFOLD_OPTIMIZER_H
#pragma once

#include "quantum/data.hpp"
#include <vector>

namespace sep::quantum {

class QuantumManifoldOptimizer {
public:
    static std::vector<pattern::PatternData>
    optimize(const std::vector<pattern::PatternData>& patterns);
};

} // namespace sep::quantum

#endif // SEP_QUANTUM_MANIFOLD_OPTIMIZER_H
