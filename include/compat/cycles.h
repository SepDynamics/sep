#pragma once

// Forward declarations and includes
#include "macros.h"

// This header provides compatibility for Cycles integration
// When SEP_HAS_CYCLES is defined, it will use the actual Cycles headers
// Otherwise, it provides stub implementations

#ifdef SEP_HAS_CYCLES
// Use the actual Cycles headers
#include "../extern/cycles/src/scene/scene.h"
#include "../extern/cycles/src/session/session.h"
#include "../extern/cycles/src/scene/film.h"
#include "../extern/cycles/src/scene/integrator.h"
#include "../extern/cycles/src/scene/camera.h"
#include "../extern/cycles/src/scene/mesh.h"
#include "../extern/cycles/src/scene/object.h"
#include "../extern/cycles/src/scene/light.h"
#endif

// Note: We don't use 'using namespace ccl' here to avoid namespace pollution
// Instead, we use explicit ccl:: prefixes in the code