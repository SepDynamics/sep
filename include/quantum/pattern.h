#pragma once

#include "quantum/types.h"
#include <complex>
#include <vector>
#include <string>

namespace sep::quantum::manifold {

enum class ManifoldQuantumState {
    SUPERPOSITION,
    COHERENT,
    COLLAPSED
};

struct QuantumPattern {
    std::vector<double> position;
    double coherence;
    double stability;
    int generation;
    ManifoldQuantumState state;
    double phase;
    std::complex<double> amplitude;
};

} // namespace sep::quantum::manifold
