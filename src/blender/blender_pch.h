#pragma once

// 1. C Standard Library Headers
// These must be first to ensure C functions are declared in the global namespace
// before C++ wrappers try to import them into std::.
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring> // For memcpy, strlen, etc.
#include <ctime>

// 2. Core C++ Standard Library Headers
#include <algorithm>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

// 3. GLM (often acts like a system-level library)
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// 4. Heavy Third-Party Libraries (Alphabetical)
#include <lz4.h>
#include <OpenColorIO/OpenColorIO.h>
#include <OpenImageIO/imageio.h>
#include <spdlog/spdlog.h>
#include <zstd.h>

// 5. Cycles Headers (Essential ones)
#include "scene/scene.h"
#include "util/math.h"
#include "util/system.h"
#include "util/types.h"

// 6. Project Core Headers (Your own stable, foundational headers)
#include "compat/shim.h"
#include "core/common.h"
#include "memory/types.h"
#include "quantum/types.h"
