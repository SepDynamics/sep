#pragma once

// CyclesRenderer compatibility header
// This provides a type alias to use SimpleRenderer in place of the missing CyclesRenderer

#include "simple_renderer.h"

namespace sep {
    // Type alias: CyclesRenderer is now just another name for SimpleRenderer
    using CyclesRenderer = SimpleRenderer;
}