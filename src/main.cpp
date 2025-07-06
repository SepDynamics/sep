#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include "core/engine.h"
#include "quantum/processor.h"
#include "quantum/types.h"
#include "core/manager.h"

int main() {
    std::cout << "--- SEP Engine Core Test ---" << std::endl;

    try {
        sep::config::ConfigManager::getInstance().initialize(0, nullptr);
        auto& engine = sep::core::Engine::instance();
        engine.init({});

        auto q_processor = sep::quantum::createProcessor({});

        sep::quantum::Pattern p;
        p.id = "genesis_pattern_1";
        p.quantum_state.coherence = 0.5f;
        p.quantum_state.stability = 0.5f;
        p.quantum_state.entropy = 0.1f;

        q_processor->addPattern(p);

        std::cout << "Initial Pattern State:" << std::endl;
        std::cout << "  Coherence: " << p.quantum_state.coherence << std::endl;
        std::cout << "  Stability: " << p.quantum_state.stability << std::endl;

        std::cout << "\n--- EVOLUTION LOOP ---" << std::endl;
        for (int i = 0; i < 50; ++i) {
            q_processor->processPattern(p.id);
            sep::quantum::Pattern current_pattern = q_processor->getPattern(p.id);
            printf("Step %02d -> Coherence: %f, Stability: %f\n",
                   i + 1,
                   current_pattern.quantum_state.coherence,
                   current_pattern.quantum_state.stability);
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        std::cout << "----------------------" << std::endl;

        sep::quantum::Pattern final_pattern = q_processor->getPattern(p.id);

        std::cout << "\n--- MEANINGFUL RESULT ---" << std::endl;
        std::cout << "Final Pattern State after 50 evolutions:" << std::endl;
        std::cout << "  Coherence: " << final_pattern.quantum_state.coherence << std::endl;
        std::cout << "  Stability: " << final_pattern.quantum_state.stability << std::endl;
        std::cout << "-------------------------" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
