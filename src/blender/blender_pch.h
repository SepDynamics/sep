#pragma once

// 1. C Standard Library Headers
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>

// 2. Core C++ Standard Library Headers
#include <algorithm>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

// 3. GLM
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
#include "session/scene.h"
#include "session/output_driver.h"
#include "util/math.h"
#include "util/system.h"
#include "util/types.h"

// 6. Project Core Headers (Your own stable, foundational headers)
#include "compat/shim.h"
#include "core/common.h"
#include "memory/types.h"
#include "quantum/types.h"
