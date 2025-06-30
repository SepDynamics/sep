#pragma once

#include <vector>
#include <glm/glm.hpp>

namespace sep::quantum::cycles {

struct QuantumScene {
    double coherence;
    double stability;
    double entropy;
    double complexity;
    int qbsa_state;
    int qfh_level;
};

struct EvolutionResult {
    double final_coherence;
    double final_stability;
    double final_entropy;
    double final_complexity;
    
    struct TrajectoryPoint {
        double coherence;
        double field_strength;
    };
    std::vector<TrajectoryPoint> trajectory;
    
    bool has_mesh_data = false;
    std::vector<glm::vec3> evolved_vertices;
};

class QuantumRenderer {
public:
    void initialize(bool use_gpu);
    EvolutionResult evolveQuantumPattern(const QuantumScene& scene, int iterations);
};

} // namespace sep::quantum::cycles