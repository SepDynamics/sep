#pragma once

// 1. ABSOLUTELY FIRST: C Standard Library Headers
// These provide fundamental functions (memcpy, strlen, etc.) in the
// global namespace. Using the <c...> variants ensures proper namespace
// pollution avoidance with modern compilers.
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>

// 2. Core C++ Standard Library Headers
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <stdexcept>

// 3. Heavy Third-Party Libraries (Alphabetical)
// These often include their own complex set of headers.
#include <OpenColorIO/OpenColorIO.h>
#include <OpenImageIO/imageio.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <lz4.h>
#include <spdlog/spdlog.h>
#include <zstd.h>

// 4. Cycles Headers (Essential ones)
#include "util/system.h"
#include "util/types.h"
#include "util/math.h"
#include "scene/scene.h"

// 5. Project Core Headers (Your own stable headers)
#include "core/common.h"
#include "quantum/types.h"
#include "memory/types.h"
#include "compat/shim.h"
