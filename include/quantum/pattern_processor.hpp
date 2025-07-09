#pragma once

#include <memory>
#include <vector>

#include "core/engine.h"

namespace sep
{
    namespace workbench
    {

        struct QuantumState
        {
            float evolution_rate = 0.5f;
            float energy_level = 1.0f;
            float coupling_strength = 0.5f;
            std::vector<int> dimensions = {32, 32, 32};
        };

        struct EvolutionResult
        {
