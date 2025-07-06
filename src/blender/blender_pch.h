#pragma once

// ---------------------------------------------------------------------------
// 1. ABSOLUTELY FIRST: C Standard Library Headers
// These must come before any third-party includes to avoid namespace pollution.
// ---------------------------------------------------------------------------
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <unistd.h>

// ---------------------------------------------------------------------------
// 2. Core C++ Standard Library Headers
// ---------------------------------------------------------------------------
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <stdexcept>

// 2. Heavy Third-Party Libraries (Alphabetical)
// These often include their own complex set of headers.
#include <OpenColorIO/OpenColorIO.h>
#include <OpenImageIO/imageio.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <lz4.h>
#include <spdlog/spdlog.h>
#include <zstd.h>

// 3. Cycles Headers (Essential ones)
#include "util/system.h"
#include "util/types.h"
#include "util/math.h"
#include "scene/scene.h"

// 4. Project Core Headers (Your own stable headers)
#include "core/common.h"
#include "quantum/types.h"
#include "memory/types.h"
#include "compat/shim.h"
