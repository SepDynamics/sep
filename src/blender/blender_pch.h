#pragma once

// 1. ABSOLUTE FIRST: Core C/C++ Standard Libraries
#include <cstring>   // memcpy, memset, strcmp, strlen
#include <ctime>     // time_t, clock_gettime, etc.
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cmath>     // math functions
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <numeric>
#include <iostream>
#include <stdexcept>
#include <array>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <chrono>

// 2. Heavy Third-Party Libraries
#include <OpenImageIO/imageio.h>
#include <OpenColorIO/OpenColorIO.h>
#include <spdlog/spdlog.h>
#include <lz4.h>
#include <zstd.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// 3. Cycles Headers (commonly used)
#include "util/system.h"
#include "util/types.h"
#include "util/math.h"
#include "scene/scene.h"

// 4. Project Core Headers
#include "core/common.h"
#include "quantum/types.h"
#include "memory/types.h"
#include "compat/shim.h"
