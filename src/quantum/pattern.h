#pragma once

#include <complex>
#include <string>
#include <vector>

#include "engine/types.h"

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
