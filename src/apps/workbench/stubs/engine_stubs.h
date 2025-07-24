#pragma once

// This file is deprecated - use real engine includes instead
// Only kept for reference during migration

#include "engine/engine.h"
#include "quantum/types.h"
#include "memory/quantum_coherence_manager.h"

// Legacy aliases for compatibility during migration
namespace sep {
    // Use real engine instead of stubs
    using Engine = sep::core::Engine;
}
